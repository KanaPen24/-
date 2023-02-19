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
#include "scenePrologue.h"
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
	const LPCWSTR g_pszPathTexBG = L"data/texture/Prologue.jpg";
}
static ID3D11ShaderResourceView*	g_pTextures[1];			// テクスチャへのポインタ

CPrologue::CPrologue() : CScene()
{
	m_id = SCENE_PROLOGUE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CPrologue::~CPrologue()
{

}

//=============================================================================
// ルール画面の初期化処理
//=============================================================================
bool CPrologue::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}


	return hr;
}

//=============================================================================
// ルール画面の終了処理
//=============================================================================
void CPrologue::Fin(void)
{

	// テクスチャ解放
	SAFE_RELEASE(m_pTexBG);
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ルール画面の更新処理
//=============================================================================
void CPrologue::Update(void)
{
	// セレクト画面へ
	if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//エンター入力
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_TITLE);//セレクトに移動
		return;
	}
	// デバック用シーン遷移
	if (CInput::GetKeyRelease(VK_1)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_TITLE);	//タイトル画面へ
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_RULE);	//ルール画面へ
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_SELECT);	//セレクト画面へ
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//エンター入力
		CSound::Play(SE_SELECT);	//効果音
		CFade::Out(SCENE_GAME);	//ゲーム画面へ
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
void CPrologue:: Draw(void)
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
