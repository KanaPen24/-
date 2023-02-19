/*=====ヘッダコメント=====
*@プログラム名	：Game.cpp
*@説明・概要	：ゲームシーンについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/17
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "Game.h"
#include "Score.h"
#include "Radar.h"
#include "Pause.h"
#include "PostProcess.h"


// コンストラクタ
CGame::CGame() : CScene()
{
	m_id = SCENE_GAME;

	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
	m_pPause = nullptr;
	m_pRadar = nullptr;
	m_pScore = nullptr;
	m_pSSelect = nullptr;

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

	//変数初期化
	m_bResult = false;
	m_bPause = false;
	m_fAlf = 0.0f;

	CCamera::Set(&m_camera);

	new CLand(this);

	new CCity(this);

	m_pSSelect = new CSSelect;	

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	switch (m_pSSelect->GetStageSelect())
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
		mW._41 = - 900.0f + (float)(rand() % 1000);
		mW._42 = 17.0f;
		mW._43 = (float)(200+rand() % 1000);
		m_pObject[i]->SetWorld(mW);
	}

	//スコア初期化
	m_pScore = new CScore;
	hr =m_pScore->Init(this);
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("スコア初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//一時停止初期化
	m_pPause = new CPause;
	hr = m_pPause->Init();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("ポーズ初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
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

	//ポストプロセス初期化
	CPostProcess::Init();

	// BGM再生開始&音量設定
	CSound::Play(BGM_GAME);
	CSound::SetVolume(SE_SHIZUKU, 0.2f);
	CSound::SetVolume(SE_CANCEL, 0.2f);
	return true;
}

// 終了処理
void CGame::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAME);

	//スコア終了処理
	m_pScore->Fin();

	//ポーズ終了
	m_pPause->Fin();

	// レーダー テクスチャ解放
	CRadar::ReleaseTexture();

	// 全オブジェクト終了処理
	CGameObj::FinAll(m_pObj);
}

// 更新
void CGame::Update()
{
	//一時停止中？
	if (m_bPause) {
		//一時停止更新
		m_pPause->Update();
	}
	else
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
	}
	//一時停止ON/OFF
	if (CInput::GetKeyTrigger(VK_Q) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON8)) {
		//フェードイン/アウト中でなければ
		if (CFade::Get() == FADE_NONE) {
			m_bPause = !m_bPause;
			if (m_bPause) {
				//一時停止開始
				CSound::Pause();
				CSound::Play(SE_SHIZUKU);
				if (m_bSoundPause == true)	//押したら一回だけ再生
				{
					m_bSoundPause = false;
				}
			}
			else
			{
				//一時停止解除
				m_fAlf = 0.0f;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				if (m_bSoundPause == false)  //押したら一回だけ再生
				{
					m_bSoundPause = true;
				}

			}
		}
	}
	//一時停止メニューの選択
	if (m_bPause&&CFade::Get() == FADE_NONE) {
		//[Enter]が押された？
		if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {
			//選択中のメニュー項目により分岐
			switch (m_pPause->GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				CFade::Out(SCENE_GAME);
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;
			case PAUSE_MENU_SELECT:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				CFade::Out(SCENE_SELECT);
				break;
			case PAUSE_MENU_RULE:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				m_fAlf = 1.0f;

				break;
			}
		}
	}

	//ポストプロセス更新
	CPostProcess::Update();

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

	//ポストプロセス描画
	CPostProcess::Draw();

	//一時停止描画
	if (m_bPause) {
		m_pPause->Draw();
	}
	
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
