/*=====ヘッダコメント=====
*@プログラム名	：SceneSelect.cpp
*@説明・概要	：ステージセレクトシーンについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneSelect.h"
#include "texture.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Debugproc.h"
#include "StegeSelect.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const float POS_X_BG = 0.0f;			//背景のX座標
const float POS_Y_BG = 0.0f;			//背景のY座標
const int WIDTH_BG = SCREEN_WIDTH;		//横幅
const int HEIGHT_BG = SCREEN_HEIGHT;	//立幅
const int MAX_TEXTURE = 1;				//使う画像数
const int BLINK_TIMER = 30;			//点滅時間で使用
const int BLINK_START_TIMER = 5;			//点滅開始時間

//*****************************************************************************
// グローバル宣言
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexBG = L"data/texture/rule.jpg";
}

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];


CSelect::CSelect() : CScene()
{
	m_id = SCENE_SELECT;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CSelect::~CSelect()
{

}

//=============================================================================
// セレクトシーンの初期化処理
//=============================================================================
bool CSelect::Init() 
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	// 変数初期化
	m_nBlink = 0;
	m_nStart = 0;
	//一回だけ再生初期化
	bSound1 = false;
	bSound2 = false;
	bSound3 = false;
	// BGM再生
	CSound::SetVolume(BGM_TITLE, 0.1f, 0.0f);
	CSound::Play(BGM_TITLE);
	//一時停止初期化
	hr = InitSSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("ステージセレクト初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return true;
}
//=============================================================================
// セレクトシーンの終了処理
//=============================================================================
void CSelect::Fin()
{
	// 音停止
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	SAFE_RELEASE(m_pTexBG);
	//ステージセレクト終了
	UninitSSelect();
}
//=============================================================================
// セレクトシーンの更新処理
//=============================================================================
void CSelect::Update() {
	//ステージセレクト更新
	UpdateSSelect();
	
	switch (GetStageSelect())
	{
	case STAGE_1:
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
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//エンター入力
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			CFade::Out(SCENE_GAME);	//ゲーム1画面へ
			m_nStart = 1; // 開始フラグ
			m_nBlink = BLINK_START_TIMER;
			return;
		}
		break;
	case STAGE_2:
		
		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//エンター入力
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			CFade::Out(SCENE_GAME);	//ゲーム画面へ
			m_nStart = 1; // 開始フラグ
			m_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	case STAGE_3:
		bSound1 = true;
		bSound2 = true;
		if (bSound3 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound3 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//エンター入力
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//効果音
			CFade::Out(SCENE_GAME);	//ゲーム画面へ
			m_nStart = 1; // 開始フラグ
			m_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	}
	//操作説明画面
	if (CInput::GetKeyPress(VK_X) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON4)) //Y押したら
	{
		CFade::Out(SCENE_RULE);	//ルール画面へ
		m_nStart = 1; // 開始フラグ
		m_nBlink = BLINK_START_TIMER;
		return;
	}
	// デバック用シーン遷移
	if (CInput::GetKeyRelease(VK_1)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_TITLE);	//タイトル画面へ
		m_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_RULE);	//ルール画面へ
		m_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_SELECT);	//セレクト画面へ
		m_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_GAME);	//ゲーム画面へ
		m_nStart = 1; // 開始フラグ
		return;
	}

#ifdef _DEBUG
	CDebugProc::Print("ﾀｲﾄﾙ:1\n");
	CDebugProc::Print("ﾙｰﾙ  :2\n");
	CDebugProc::Print("ｾﾚｸﾄ :3\n");
	CDebugProc::Print("ｹﾞｰﾑ :4\n");
#endif // _DEBUG

}
//=============================================================================
// セレクトシーンの描画処理
//=============================================================================
void  CSelect::Draw() {

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(POS_X_BG, POS_Y_BG);
	CPolygon::Draw(pDC);

	//ステージセレクト描画
	DrawSSelect();

}