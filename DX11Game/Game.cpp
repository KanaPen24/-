//=============================================================================
//
// ゲーム クラス実装 [Game.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Game.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "City.h"
#include "Score.h"

// コンストラクタ
CGame::CGame() : CScene()
{
	m_id = SCENE_GAME;

	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
	for (int i = 0; i < _countof(m_pObject); ++i)
		m_pObject[i] = nullptr;

	m_nBoundary = 0;
}

// デストラクタ
CGame::~CGame()
{
}

// 初期化
bool CGame::Init()
{
	HRESULT hr = S_OK;

	m_bResult = false;

	m_camera.Init();
	CCamera::Set(&m_camera);

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}

	new CLand(this);

	new CCity(this);

	m_pPlayer = new CPlayer(this);

	m_pEnemy = new CEnemy(this);

	for (int i = 0; i < MAX_OBJECT; i++) {

		m_pObject[i] = new CObject(this);
		m_pObject[i]->SetPlayer(m_pPlayer);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixRotationRollPitchYaw(0.0f,XMConvertToRadians(rand() % 360),0.0f));
		mW._41 = - 400.0f + (float)(rand() % 400);
		mW._43 = (float)(rand() % 800);
		m_pObject[i]->SetWorld(mW);
	}

	//スコア初期化
	hr = InitScore();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("スコア初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// 全オブジェクト初期化
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	m_camera.SetPlayer(m_pPlayer);

	// BGM再生開始
	CSound::Play(BGM_GAME);

	return true;
}

// 終了処理
void CGame::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAME);

	//スコア終了処理
	UninitScore();

	// 全オブジェクト終了処理
	CGameObj::FinAll(m_pObj);
}

// 更新
void CGame::Update()
{
	// 全キャラ更新
	CGameObj::UpdateAll(m_pObj);

	if (!m_bResult) {
		if (CInput::GetKeyRelease(VK_BACK) ||
			CInput::GetKeyRelease(VK_DELETE)) {
			m_bResult = true;
			CFade::Out(SCENE_TITLE);
		}
	}

	// スコアの更新
	UpdateScore();

#ifdef _DEBUG
	//static LPCSTR boundary[] = {"ﾋﾋｮｳｼﾞ", "ｷｭｳ"};
	//CDebugProc::Print("[B] : ｷｮｳｶｲ %s\n", boundary[m_nBoundary]);
	//CDebugProc::Print("\n");
#endif // !_DEBUG

	
}

// 描画
void CGame::Draw()
{
	// 全キャラ描画
	CGameObj::DrawAll(m_pObj);

	// スコア描画
	DrawScore();
}

// 境界表示更新
void CGame::UpdateBoundary()
{
	for (CGameObj* pObj = GetObj(); pObj; pObj = pObj->GetNext()) {
		switch (pObj->GetID()) {
		case GOT_PLAYER:
			switch (m_nBoundary) {
			case 0:	// 境界表示無し
				pObj->SetBSphereVisible(false);
				break;
			case 1:	// 境界球
				pObj->SetBSphereVisible();
				break;
			}
			break;
		}
	}
}
