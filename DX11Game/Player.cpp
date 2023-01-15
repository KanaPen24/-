//=============================================================================
//
// プレイヤー クラス実装 [Player.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Player.h"
#include "Input.h"
#include "Debugproc.h"
#include "GameCamera.h"
#include "Scene.h"
#include <time.h>

// 定数定義
namespace {
	enum EAnimNo {
		ANIM_IDLE = 0,
		ANIM_WALK,
		ANIM_RUN,
		MAX_ANIMNO
	};
}

const float RATE_MOVE_MODEL = 0.25f;			// 移動慣性係数
const float SPEED = 2.0f;						// 移動スピード
const float SCALE = 14.0f;						// 大きさ
const float DASH = 1.5f;						// ダッシュ
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	// 左スティックのデッドゾーン
//const REV_Z_AXIS	// Y軸180度回転

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_PLAYER;
	m_uTick = 0;
	m_nSpeed = 0;
	m_pCity = nullptr;
	m_bCollision = false;
	XMStoreFloat4x4(&m_mInvWorld, XMMatrixIdentity());
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 初期化
HRESULT CPlayer::Init()
{
	HRESULT hr = CModel::Init();
	if (SUCCEEDED(hr)) {
		SetModel(MODEL_PLAYER);
		m_uTick = 0;
		m_pCity = (CCity*)m_pScene->Find(GOT_CITY);
		m_naAnimNo = ANIM_IDLE;
		m_dAnimTime = 0.0;
	}
	CAssimpModel* pModel = GetAssimp(MODEL_ENEMY);
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	return hr;
}



// 更新
void CPlayer::Update()
{
	//変数宣言
	float fDash = 1.0f;

	CModel::Update();

	// 現在位置取得
	XMFLOAT3 vPos = GetPos();
	m_nSpeed = 0;
	// 回転
	XMFLOAT3 angle = GetAngle();
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();
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

		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// 左前移動
				vPos.x -= SinDeg(rotCamera.y + 135.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y + 135.0f) * SPEED * fDash;

				angle.y = rotCamera.y + 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// 左後移動
				vPos.x -= SinDeg(rotCamera.y + 45.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y + 45.0f) * SPEED * fDash;

				angle.y = rotCamera.y + 45.0f;
			}
			else {
				// 左移動
				vPos.x -= SinDeg(rotCamera.y + 90.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y + 90.0f) * SPEED * fDash;

				angle.y = rotCamera.y + 90.0f;
			}
			m_nSpeed = 1;
		}
		else if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE) {
			if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
				// 右前移動
				vPos.x -= SinDeg(rotCamera.y - 135.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y - 135.0f) * SPEED * fDash;

				angle.y = rotCamera.y - 135.0f;
			}
			else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
				// 右後移動
				vPos.x -= SinDeg(rotCamera.y - 45.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y - 45.0f) * SPEED * fDash;

				angle.y = rotCamera.y - 45.0f;
			}
			else {
				// 右移動
				vPos.x -= SinDeg(rotCamera.y - 90.0f) * SPEED * fDash;
				vPos.z -= CosDeg(rotCamera.y - 90.0f) * SPEED * fDash;

				angle.y = rotCamera.y - 90.0f;
			}
			m_nSpeed = 1;
		}
		else if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			// 前移動
			vPos.x -= SinDeg(180.0f + rotCamera.y) * SPEED * fDash;
			vPos.z -= CosDeg(180.0f + rotCamera.y) * SPEED * fDash;

			angle.y = 180.0f + rotCamera.y;
			m_nSpeed = 1;
		}
		else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
			// 後移動
			vPos.x -= SinDeg(rotCamera.y) * SPEED * fDash;
			vPos.z -= CosDeg(rotCamera.y) * SPEED * fDash;

			angle.y = rotCamera.y;
			m_nSpeed = 1;
		}
	}

	// タイマ更新
	++m_uTick;
	//キー入力処理
	//ダッシュ処理
	if (CInput::GetKeyPress(VK_LSHIFT)) fDash = DASH;
	
	if (CInput::GetKeyPress(VK_LEFT)|| CInput::GetKeyPress(VK_A)) {
		if (CInput::GetKeyPress(VK_UP)|| CInput::GetKeyPress(VK_W)) {
			// 左前移動
			vPos.x -= SinDeg(rotCamera.y + 135.0f) * SPEED * fDash;
			vPos.z -= CosDeg(rotCamera.y + 135.0f) * SPEED * fDash;

			angle.y = rotCamera.y + 135.0f;
		}
		else if (CInput::GetKeyPress(VK_DOWN)|| CInput::GetKeyPress(VK_S)) {
			// 左後移動
			vPos.x -= SinDeg(rotCamera.y + 45.0f) * SPEED * fDash;
			vPos.z -= CosDeg(rotCamera.y + 45.0f) * SPEED * fDash;

			angle.y = rotCamera.y + 45.0f;
		}
		else {
			// 左移動
			vPos.x -= SinDeg(rotCamera.y + 90.0f) * SPEED * fDash;
			//vPos.z -= CosDeg(rotCamera.y + 90.0f) * SPEED * fDash;

			angle.y = rotCamera.y + 90.0f;
		}
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_RIGHT)|| CInput::GetKeyPress(VK_D)) {
		if (CInput::GetKeyPress(VK_UP)|| CInput::GetKeyPress(VK_W)) {
			// 右前移動
			vPos.x -= SinDeg(rotCamera.y - 135.0f) * SPEED * fDash;
			vPos.z -= CosDeg(rotCamera.y - 135.0f) * SPEED * fDash;

			angle.y = rotCamera.y - 135.0f;
		}
		else if (CInput::GetKeyPress(VK_DOWN)|| CInput::GetKeyPress(VK_S)) {
			// 右後移動
			vPos.x -= SinDeg(rotCamera.y - 45.0f) * SPEED * fDash;
			vPos.z -= CosDeg(rotCamera.y - 45.0f) * SPEED * fDash;

			angle.y = rotCamera.y - 45.0f;
		}
		else {
			// 右移動
			vPos.x -= SinDeg(rotCamera.y - 90.0f) * SPEED * fDash;

			angle.y = rotCamera.y - 90.0f;
		}
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_UP)|| CInput::GetKeyPress(VK_W)) {
		// 前移動
		vPos.z -= CosDeg(180.0f + rotCamera.y) * SPEED * fDash;

		angle.y = 180.0f + rotCamera.y;
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_DOWN)|| CInput::GetKeyPress(VK_S)) {
		// 後移動
		vPos.z -= CosDeg(rotCamera.y) * SPEED * fDash;

		angle.y = rotCamera.y;
		m_nSpeed = 1;
	}
	if (CInput::GetKeyPress(VK_LSHIFT)) m_nSpeed = 2;

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
	
	// アニメーション更新
	if (m_nSpeed == 0)
		m_naAnimNo = ANIM_IDLE;
	if (m_nSpeed == 1)
		m_naAnimNo = ANIM_WALK;
	if (m_nSpeed == 2)
		m_naAnimNo = ANIM_RUN;
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	m_dAnimTime += dSlice;
	if (m_dAnimTime >= GetModel()->GetAnimDuration(m_naAnimNo)) {
		m_dAnimTime = 0.0;
	}
#ifdef _DEBUG
	//デバック表示
	CDebugProc::Print("SPEED * fDash=%f\n", m_nSpeed);
	CDebugProc::Print("Animation=%d\n", m_naAnimNo);
	CDebugProc::Print("ﾌﾟﾚｲﾔｰPos=X:%f Y:%f Z:%f\n", vPos.x, vPos.y, vPos.z);
#endif
}
// 描画
void CPlayer::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 1);
	CAssimpModel* pModel = GetModel();
	if (pModel) {
		pModel->SetAnimIndex(m_naAnimNo);
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
		pModel->SetAnimIndex(m_naAnimNo);
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

