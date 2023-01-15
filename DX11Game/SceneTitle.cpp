//=============================================================================
//
// タイトル画面クラス実装 [Title.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "SceneTitle.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"
#include "TitleSelect.h"
#include "Land.h"
#include "Particle.h"

// 定数定義
namespace {
	const LPCWSTR g_pszPathTexLogo = L"data/texture/title2.png";

	const LPCWSTR g_pszPathTexBG = L"data/texture/sky001.jpg";

}

static bool bSound1;	//一回だけ流す
static bool bSound2;	//一回だけ流す
static bool bSound3;	//一回だけ流す

// コンストラクタ
CTitle::CTitle() : CScene()
{
	m_id = SCENE_TITLE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexBG = nullptr;	
}

// デストラクタ
CTitle::~CTitle()
{
}

// 初期化
bool CTitle::Init()
{
	if (!CScene::Init()) {
		return false;
	}
	m_camera.Init();
	CCamera::Set(&m_camera);
	
	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexLogo, &m_pTexTitle);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	//タイトルセレクト初期化
	hr = InitTSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("タイトルセレクト初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	new CLand(this);
	m_pCity = new CCity(this);
	// 全オブジェクト初期化
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}
	//カメラを街に向ける
	m_camera.SetCity(m_pCity);

	// BGM再生開始
	CSound::Play(BGM_TITLE);
	CSound::SetVolume(BGM_TITLE, 0.2f, 0.0f);
	return true;
}

// 終了処理
void CTitle::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_TITLE);

	SAFE_RELEASE(m_pTexBG);
	SAFE_RELEASE(m_pTexTitle);

	//タイトルセレクト終了
	UninitTSelect();

	// 全オブジェクト終了処理
	CGameObj::FinAll(m_pObj);

	CScene::Fin();
}

// 更新
void CTitle::Update()
{
	//タイトルセレクト更新
	UpdateTSelect();

	//パーティクル更新
	UpdateParticle();

	CSound::SetVolume(SE_SELECT, 0.1f);
	switch (GetTitleSelect())
	{
	case GAME:
		bSound2 = true;
		bSound3 = true;
		//CSound::Resume();
		if (bSound1 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound1 = false;
		}
		// 開始
		if (CInput::GetKeyRelease(VK_RETURN)) {//){ || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::Play(SE_SELECT);	//効果音
			CFade::Out(SCENE_SELECT);	//ゲーム1画面へ
			
		}
		break;
	case END:

		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN)) {//){ || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
			CSound::Play(SE_SELECT);	//効果音
			PostQuitMessage(0);				// システムにスレッドの終了を要求
		}
		break;
	}
	// 全キャラ更新
	CGameObj::UpdateAll(m_pObj);
}

// 描画
void CTitle::Draw()
{
	HDC hdc;
	PAINTSTRUCT	ps;
	HWND hWnd = GetMainWnd();
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	//背景描画
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);
	SetZBuffer(true);
	SetBlendState(BS_NONE);

	// モデル描画
	CGameObj::DrawAll(m_pObj);
	
	//パーティクル描画
	DrawParticle();

	//タイトルロゴ描画
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetTexture(m_pTexTitle);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::Draw(pDC);

	//タイトルセレクト描画
	DrawTSelect();
	
	// パーティクル描画
	/*hdc = BeginPaint(hWnd, &ps);
	CParticle::ExecParticle();
	CParticle::DrawParticle(hdc);
	EndPaint(hWnd, &ps);*/
	SetZBuffer(true);

}
