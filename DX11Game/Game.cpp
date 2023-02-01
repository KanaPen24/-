/*=====ヘッダコメント=====
*@プログラム名	：Game.cpp
*@説明・概要	：ゲームシーンについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/17
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "Game.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "City.h"
#include "Score.h"
#include "Radar.h"
#include "StegeSelect.h"

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

	CCamera::Set(&m_camera);

	new CLand(this);

	new CCity(this);

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	switch (GetStageSelect())
	{
	case STAGE_1:
		m_nCntObj = 1;
		break;
	case STAGE_2:
		m_nCntObj = 10;
		break;
	case STAGE_3:
		m_nCntObj = 20;
		break;
	}	

	m_pPlayer = new CPlayer(this);

	m_pEnemy = new CEnemy(this);
	m_pEnemy->SetPlayer(m_pPlayer);
	

	for (int i = 0; i < m_nCntObj; i++) {

		m_pObject[i] = new CObject(this);
		m_pObject[i]->SetPlayer(m_pPlayer);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixRotationRollPitchYaw(0.0f,XMConvertToRadians(rand() % 360),0.0f));
		mW._41 = - 500.0f + (float)(rand() % 500);
		mW._43 = (float)(200+rand() % 800);
		m_pObject[i]->SetWorld(mW);
	}

	//スコア初期化
	m_pScore = new CScore;
	hr =m_pScore->Init(this);
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("スコア初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// レーダー テクスチャ読込
	if (FAILED(CRadar::LoadTexture())) {
		return false;
	}

	// 全オブジェクト初期化
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}
	
	//カメラプレイヤーに設定
	m_camera.SetPlayer(m_pPlayer);
	m_camera.Init();

	// レーダー生成、初期化
	m_pRadar = new CRadar();
	m_pRadar->Init(this);

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
	m_pScore->Uninit();

	// レーダー テクスチャ解放
	CRadar::ReleaseTexture();

	m_camera.Fin();

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

	// レーダー更新
	m_pRadar->Update();

	// スコアの更新
	m_pScore->Update();

#ifdef _DEBUG
	//static LPCSTR boundary[] = {"ﾋﾋｮｳｼﾞ", "ｷｭｳ"};
	//CDebugProc::Print("[B] : ｷｮｳｶｲ %s\n", boundary[m_nBoundary]);
	//CDebugProc::Print("\n");
#endif // !_DEBUG

	
}

// 描画
void CGame::Draw()
{
	SetBlendState(BS_ALPHABLEND);
	// 全キャラ描画
	CGameObj::DrawAll(m_pObj);

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	// スコア描画
	m_pScore->Draw();
	// レーダー描画
	m_pRadar->Draw();
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
