/*=====ヘッダコメント=====
*@プログラム名	：Player.cpp
*@説明・概要	：プレイヤーのモデル及び挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "Player.h"
#include "Input.h"
#include "Debugproc.h"
#include "GameCamera.h"
#include "Scene.h"
#include <time.h>

// マクロ定義
namespace {
	enum EAnimNo {
		ANIM_IDLE = 0,
		ANIM_WALK,
		ANIM_RUN,
		MAX_ANIMNO
	};
}
const float RATE_MOVE_MODEL = 0.25f;			// 移動慣性係数
const float SPEED = 0.02f;						// 移動スピード
const float SCALE = 1.0f;						// 大きさ
const float DASH = 1.5f;						// ダッシュ
const float ROTATE = 1.7f;		// カメラの距離
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	// 左スティックのデッドゾーン
//#define REV_Z_AXIS	// Y軸180度回転

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_PLAYER;
	m_uTick = 0;
	m_nSpeed = 0;
	m_pCity = nullptr;
	m_bCollision = false;
	m_pGCam = nullptr;
	XMStoreFloat4x4(&m_mInvWorld, XMMatrixIdentity());
}

// デストラクタ
CPlayer::~CPlayer()
{
	m_id = GOT_PLAYER;
	XMStoreFloat4x4(&m_mInvWorld, XMMatrixIdentity());
}

// 初期化
HRESULT CPlayer::Init()
{
	HRESULT hr = CModel::Init();
	if (SUCCEEDED(hr)) {
		SetModel(MODEL_PLAYER);
		m_uTick = 0;
		m_pCity = (CCity*)m_pScene->Find(GOT_CITY);
		m_pGCam = CCamera::Get();
		m_nAnimNo = ANIM_IDLE;
		m_dAnimTime = 0.0;
	}
	CAssimpModel* pModel = GetAssimp(MODEL_PLAYER);
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	SetScale(XMFLOAT3(SCALE, SCALE, SCALE));
	SetAngle(XMFLOAT3(0.0f, 180.0f, 0.0f));	//初期で奥を向く
	return hr;
}

// 更新
void CPlayer::Update()
{
	//変数宣言
	float fDash = 1.0f;
	float fLength = m_pGCam->GetLength();
	CModel::Update();
	XMFLOAT3 vVec = m_pGCam->GetVec();
	vVec.y = 0.0f;

	// 現在位置取得
	XMFLOAT3 vPos = GetPos();
	m_nSpeed = 0;
	// 回転
	XMFLOAT3 angle = GetAngle();
	// カメラの向き取得
	XMFLOAT3 rotCamera = m_pGCam->GetAngle();
	//ダッシュ処理
	if (CInput::GetKeyPress(VK_LSHIFT) && (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W) || CInput::GetKeyPress(VK_DOWN)
		|| CInput::GetKeyPress(VK_S)))	fDash = DASH;
	//ゲームコントローラー
	DWORD JoyCount = CInput::GetJoyCount();
	CInput::GetJoyState(JOYSTICKID1);
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE) &&
		(JoyY<  GAMEPAD_LEFT_STICK_DEADZONE &&
			JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0;
		JoyY = 0;
	}
	/*ここからコントローラー操作の入力*/
	if (JoyCount >= 1) {
		if(CInput::GetJoyButton(JOYSTICKID1, JOY_BUTTON4)&& 
			(JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE|| JoyY >= GAMEPAD_LEFT_STICK_DEADZONE))
			fDash = DASH;
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			// 前移動
			vPos.x -= vVec.x* SPEED * fDash;
			vPos.z -= vVec.z* SPEED * fDash;
			m_nSpeed = 1;
		}
		else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
			// 後移動
			vPos.x += vVec.x* SPEED * fDash;
			vPos.z += vVec.z* SPEED * fDash;
			m_nSpeed = 1;
		}
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			angle.y -= ROTATE;
		}
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE) {
			angle.y += ROTATE;
		}
		if (CInput::GetJoyButton(JOYSTICKID1, JOY_BUTTON4) &&
			(JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE || JoyY >= GAMEPAD_LEFT_STICK_DEADZONE))
			m_nSpeed = 2;
	}

	// タイマ更新
	++m_uTick;

	//キー入力処理
	if (CInput::GetKeyPress(VK_UP)|| CInput::GetKeyPress(VK_W)) {
		// 前移動
		vPos.x -= vVec.x* SPEED * fDash;
		vPos.z -= vVec.z* SPEED * fDash;
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S)) {
		// 後移動
		vPos.x += vVec.x* SPEED * fDash;
		vPos.z += vVec.z* SPEED * fDash;
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_RIGHT)) {
		// 右移動
		vPos.x += SPEED * fDash * 100;
		m_nSpeed = 1;
	}
	if (CInput::GetKeyPress(VK_LEFT)) {
		// 左移動
		vPos.x -= SPEED * fDash * 100;
		m_nSpeed = 1;
	}
	if (CInput::GetKeyPress(VK_LSHIFT)&&(CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W) || CInput::GetKeyPress(VK_DOWN)
		|| CInput::GetKeyPress(VK_S)))
		m_nSpeed = 2;

	//角度の変更
	if (CInput::GetKeyPress(VK_D))
		angle.y += ROTATE;
	if (CInput::GetKeyPress(VK_A))
		angle.y -= ROTATE;

	if (angle.y >= 360.0f)
		angle.y = 0.0f;
	if (angle.y < 0.0f)
		angle.y = 360.0f;
	// 回転マトリックス生成
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, XMMatrixRotationY(XMConvertToRadians(angle.y)));
	// モデル座標軸取得
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);

	// 地面との当たり判定
	XMFLOAT3 vP0;
	vP0.x = vPos.x;
	vP0.y = vPos.y + GetRadius() * 2;
	vP0.z = vPos.z;
	XMFLOAT3 vX, vN;	//交点座標
	if (m_pCity && m_pCity->Collision(vP0,XMFLOAT3(0.0f, -1.0f, 0.0f),m_bCollision, &vX, &vN)) {
		vPos.x = vX.x;	
		vPos.z = vX.z;
		m_bCollision = false;	//当たった瞬間の座標が欲しいのでここで当たり判定を消す
	} else {
		vPos.y = 0.0f;
		m_bCollision = true;	//離れたら次に当たる瞬間備えて当たり判定を付ける
	}

	//移動反映
	mW._41 = vPos.x;
	mW._42 = vPos.y;
	mW._43 = vPos.z;
	SetPos(vPos);

	// ワールド変換行列更新
	SetWorld(mW);

	// 角度更新
	SetAngle(angle);

	// 逆行列生成
	XMStoreFloat4x4(&m_mInvWorld,
		XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_mWorld)));
	
	// アニメーション更新
	if (m_nSpeed == 0)
		m_nAnimNo = ANIM_IDLE;
	if (m_nSpeed == 1)
		m_nAnimNo = ANIM_WALK;
	if (m_nSpeed == 2)
		m_nAnimNo = ANIM_RUN;
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	m_dAnimTime += dSlice;
	if (m_dAnimTime >= GetModel()->GetAnimDuration(m_nAnimNo)) {
		m_dAnimTime = 0.0;
	}
#ifdef _DEBUG
	
	//デバック表示
	/*CDebugProc::Print("SPEED * fDash=%f\n", m_nSpeed);
	CDebugProc::Print("Animation=%d\n", m_nAnimNo);
	CDebugProc::Print("ﾌﾟﾚｲﾔｰPos=X:%f Y:%f Z:%f\n", vPos.x, vPos.y, vPos.z);
	CDebugProc::Print("ﾌﾟﾚｲﾔｰVec=X:%f Y:%f Z:%f\n", vVec.x, vVec.y, vVec.z);
	CDebugProc::Print("ﾌﾟﾚｲﾔｰAng=X:%f Y:%f Z:%f\n", angle.x, angle.y, angle.z);*/
	
#endif
}
// 描画
void CPlayer::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 1);
	CAssimpModel* pModel = GetModel();
	if (pModel) {
		pModel->SetAnimIndex(m_nAnimNo);
		pModel->SetAnimTime(m_dAnimTime);
#ifdef REV_Z_AXIS
		ID3D11DeviceContext* pDC = GetDeviceContext();
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW,
			XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)),
				XMLoadFloat4x4(&GetWorld())));
		pModel->Draw(pDC, mW, eOpacityOnly);
#else
		CModel::Draw();
#endif
	}
}

// 半透明部分描画
void CPlayer::DrawAlpha()
{
	CAssimpModel* pModel = GetModel();
	if (pModel) {
		pModel->SetAnimIndex(m_nAnimNo);
		pModel->SetAnimTime(m_dAnimTime);
#ifdef REV_Z_AXIS
		ID3D11DeviceContext* pDC = GetDeviceContext();
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW,
			XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)),
				XMLoadFloat4x4(&GetWorld())));
		pModel->Draw(pDC, mW, eTransparentOnly);
#else
		CModel::DrawAlpha();
#endif
	}
	CGameObj::DrawAlpha();
}

