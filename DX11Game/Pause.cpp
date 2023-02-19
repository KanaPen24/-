/*=====ヘッダコメント=====
*@プログラム名	：pause.cpp
*@説明・概要	：ポーズ画面などについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Pause.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const int NUM_PAUSE_MENU = 3;			// ポーズメニュー数
const float PAUSE_MENU_WIDTH = 320.0f;	// ポーズメニュー幅
const float PAUSE_MENU_HEIGHT = 60.0f;	// ポーズメニュー高さ
const float PAUSE_MENU_INTERVAL = 100.0f;			// ポーズメニュー間隔
const float PAUSE_MENU_POS_X = 0.0f;				// ポーズメニュー位置(X座標)
const float PAUSE_MENU_POS_Y = PAUSE_MENU_INTERVAL;	// ポーズメニュー位置(Y座標)
const float PLATE_WIDTH = 360.0f;		// プレートの幅
const float PLATE_HEIGHT = 340.0f;		// プレートの幅
const float PLATE_POS_X = 0.0f;			// プレートの位置(X座標)
const float PLATE_POS_Y = 0.0f;			// プレートの位置(Y座標)
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;		//左スティックのデッドゾーン
const int MAX_TEXTURE = 3;				//最大テクスチャ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[6] = {nullptr};	// テクスチャへのポインタ

static PAUSE_MENU g_nSelectPMenu = PAUSE_MENU_CONTINUE;		// 選択中のメニューNo.

static LPCWSTR c_aFileNamePauseMenu[MAX_TEXTURE] =
{
	L"data/texture/retry.png",	// リトライ
	L"data/texture/select.png",	// セレクト
	L"data/texture/rule.png",	// ルール
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNamePauseMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	m_fCol = 0.0f;
	m_fCurve = 0.0f;
	m_bJoyStick = false;	//一個ずつ動かすためのフラグ

	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Fin()
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update()
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//デッドゾーン以下ならすべて０にする
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//左スティックが上に押された
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			m_bJoyStick = true;
		}
		//左スティックが下に押された
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			m_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			m_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_W)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}
	else if ( CInput::GetKeyRepeat(VK_S)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	m_fCurve += XM_PI * 0.01f;
	if (m_fCurve > XM_PI) {
		m_fCurve -= XM_2PI;
	}

	// 反射光の設定
	m_fCol = cosf(m_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
	CPolygon::SetPos(PLATE_POS_X, PLATE_POS_Y);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(m_fCol, m_fCol, m_fCol, 1.0f);
	CPolygon::Draw(pDeviceContext);

	CPolygon::SetSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		CPolygon::SetPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectPMenu) {
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			//選ばれてないほう
			CPolygon::SetColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// テクスチャの設定
		CPolygon::SetTexture(g_pTextures[nCntPauseMenu]);
		// ポリゴンの描画
		CPolygon::Draw(pDeviceContext);
	}

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void  CPause::SetPauseMenu()
{
	m_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU CPause::GetPauseMenu()
{
	return g_nSelectPMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void CPause::ResetPauseMenu()
{
	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	CPause::SetPauseMenu();
}
