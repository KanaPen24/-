/*=====ヘッダコメント=====
*@プログラム名	：SceneRule.cpp
*@説明・概要	：ルールシーンについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "sceneRule.h"
#include "Polygon.h"
#include "Texture.h"
#include "Input.h"
#include "Fade.h"
#include "Sound.h"
#include "Scene.h"
#include "Debugproc.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const int COUNT_APPEAR = 60;		// スタートボタン出現までの時間
const int INTERVAL_DISP = 60;		// スタートボタン点滅の時間


const int BLINK_TIMER = 30;			//点滅時間で使用
const int BLINK_START_TIMER = 5;			//点滅開始時間

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexBG = L"data/texture/setumei.jpg";
}
static ID3D11ShaderResourceView*	g_pTextures[1];			// テクスチャへのポインタ
static int							g_nCountAppear = 0;		//
static int							g_nCountDisp = 0;		//
static bool							g_bDispStart = false;	//
static bool							g_bSkip = false;		// スタートボタン押下
static int g_nBlink;
static int g_nStart;

CRule::CRule() : CScene()
{
	m_id = SCENE_RULE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CRule::~CRule()
{

}

//=============================================================================
// ルール画面の初期化処理
//=============================================================================
bool CRule::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	// 変数初期化
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bSkip = false;

	// BGM再生開始
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// ルール画面の終了処理
//=============================================================================
void CRule::Fin(void)
{
	// BGM再生停止
	//CSound::Stop(BGM_TITLE);

	// テクスチャ解放
	SAFE_RELEASE(m_pTexBG);
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ルール画面の更新処理
//=============================================================================
void CRule::Update(void)
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0) {
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bSkip) {
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]またはスペースキーで開始
	//if (!g_bSkip && (CInput::GetKeyRelease(VK_RETURN) || CInput::GetKeyRelease(VK_SPACE)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1))) {
	//	// 選択音
	//	//CSound::Play(SE_SELECT);
	//	// フェードアウト後シーン遷移
	//	CFade::Out(SCENE_TITLE);	//タイトルに移動

	//	g_bSkip = true;
	//}
	// タイトル画面へ
	if (CInput::GetKeyRelease(VK_RETURN)){ //GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//エンター入力
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_SELECT);//セレクトに移動
		g_nStart = 1; // 開始フラグ
		return;
	}
	// デバック用シーン遷移
	if (CInput::GetKeyRelease(VK_1)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_TITLE);	//タイトル画面へ
		g_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_RULE);	//ルール画面へ
		g_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_SELECT);	//セレクト画面へ
		g_nStart = 1; // 開始フラグ
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_GAME);	//ゲーム画面へ
		g_nStart = 1; // 開始フラグ
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
// チュートリアル画面描画
//=============================================================================
void CRule:: Draw(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

}
