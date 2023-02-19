/*=====ヘッダコメント=====
*@プログラム名	：SceneClear.cpp
*@説明・概要	：クリア画面などについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "SceneClear.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "StegeSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const int		NUM_CLEAR_MENU = 3;			// クリアメニュー数
const float 	CLEAR_MENU_WIDTH = 320.0f;	// クリアメニュー幅
const float 	CLEAR_MENU_HEIGHT = 60.0f;		// クリアメニュー高さ
const float 	CLEAR_MENU_POS_X = -380.0f;// クリアメニュー位置(X座標)
const float 	CLEAR_MENU_POS_Y = -250.0f;	// クリアメニュー位置(Y座標)
const float 	CLEAR_MENU_INTERVAL = 400.0f;	// クリアメニュー間隔
const int 		PLATE_WIDTH = SCREEN_WIDTH;	// プレートの幅
const float 	PLATE_HEIGHT = 50.0f;		// プレートの高さ
const float 	PLATE_POS_X = 0.0f;		// プレートの位置X座標)
const int 		PLATE_POS_Y = CLEAR_MENU_POS_Y;		// プレートの位置(Y座標)
const int		GAMEPAD_LEFT_STICK_DEADZONE = 7800;	//左スティックのデッドゾーン
const int		MAX_TEXTURE = 4;
const float		POS_X_BG = 0.0f;		//背景のX座標
const float		POS_Y_BG = 0.0f;		//背景のY座標
const int		WIDTH_BG = SCREEN_WIDTH;	//横幅
const int		HEIGHT_BG = SCREEN_HEIGHT;	//立幅
const int		TEX_BG = 0;				//背景の画像数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[4] = { nullptr };	// テクスチャへのポインタ

static CLEAR_MENU g_nSelectCMenu = CLEAR_MENU_SELECT;		// 選択中のメニューNo.
static float	m_fCurve = 0.0f;
//static float	m_fCol = 0.0f;
static bool		m_bJoyStick;
static bool		g_bSound1;		// 一回だけ流す
static bool		g_bSound2;		// 一回だけ流す
static bool		g_bSound3;		// 一回だけ流す
static bool		g_bClear;		//クリアフラグ

static LPCWSTR c_aFileNameClearMenu[MAX_TEXTURE] =
{
	L"data/texture/stageclear.jpg",	// 背景
	L"data/texture/select.png",	// セレクト
	L"data/texture/retry.png",	// リトライ
	L"data/texture/Gtitle.png"
};
// コンストラクタ
CClear::CClear() : CScene()
{
	m_id = SCENE_CLEAR;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;

}

// デストラクタ
CClear::~CClear()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
bool CClear::Init(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntClearMenu = 0; nCntClearMenu < MAX_TEXTURE; ++nCntClearMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameClearMenu[nCntClearMenu],	// ファイルの名前
			&g_pTextures[nCntClearMenu]);			// 読み込むメモリー
	}

	g_nSelectCMenu = CLEAR_MENU_SELECT;
	m_fCurve = 0.0f;
	m_bJoyStick = false;
	g_bClear = false;

	// 効果音初期化
	// BGM再生
	CSound::SetVolume(BGM_TITLE, 0.04f, 0.0f);
	CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CClear::Fin(void)
{
	// 音停止
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);

	// テクスチャの開放
	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		SAFE_RELEASE(g_pTextures[nCntClearMenu]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CClear::Update(void)
{
	DWORD JoyCount =CInput::GetJoyCount();
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//デッドゾーン以下ならすべて０にする
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0l;
	}

	if (JoyCount >= 1) {
		//左スティックが左に押された
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
			SetClearMenu();
			m_bJoyStick = true;
		}
		//左スティックが右に押された
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
			SetClearMenu();
			m_bJoyStick = true;
		}
		if (JoyX == 0l)
		{
			m_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_LEFT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}
	else if (CInput::GetKeyRepeat(VK_RIGHT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}

	m_fCurve += XM_PI * 0.01f;
	if (m_fCurve > XM_PI) {
		m_fCurve -= XM_2PI;
	}
	//ゲームクリアメニューの選択
	if (g_bClear&&CFade::Get() == FADE_NONE) {
		//[Enter]が押された？
		if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {
			//選択中のメニュー項目により分岐
			switch (GetClearMenu())
			{
			case CLEAR_MENU_SELECT:
				CFade::Out(SCENE_SELECT);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_RETRY:
				CFade::Out(SCENE_GAME);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_TITLE:
				CFade::Out(SCENE_TITLE);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			}
		}
		switch (GetClearMenu())
		{
		case CLEAR_MENU_SELECT:
			g_bSound2 = true;
			g_bSound3 = true;

			if (g_bSound1 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound1 = false;
			}
			break;

		case CLEAR_MENU_RETRY:
			g_bSound1 = true;
			g_bSound3 = true;

			if (g_bSound2 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound2 = false;
			}
			break;

		case CLEAR_MENU_TITLE:
			g_bSound1 = true;
			g_bSound2 = true;

			if (g_bSound3 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound3 = false;
			}
			break;
		}
	}
	//// 反射光の設定
	//m_fCol = cosf(m_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// 描画処理
//=============================================================================
void CClear::Draw(void)
{
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(WIDTH_BG, HEIGHT_BG);
	CPolygon::SetPos(POS_X_BG, POS_Y_BG);
	CPolygon::SetTexture(g_pTextures[0]);
	CPolygon::Draw(pDeviceContext);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(CLEAR_MENU_WIDTH, CLEAR_MENU_HEIGHT);

	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		CPolygon::SetPos(CLEAR_MENU_POS_X + nCntClearMenu * CLEAR_MENU_INTERVAL, CLEAR_MENU_POS_Y);
		if (nCntClearMenu == g_nSelectCMenu) {
			//選ばれてるほう
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			CPolygon::SetSize(CLEAR_MENU_WIDTH + 30.0f, CLEAR_MENU_HEIGHT + 80.0f);
		}
		else {
			//選ばれてないほう
			CPolygon::SetColor(0.3f, 0.3f, 0.3f, 0.5f);
			CPolygon::SetSize(CLEAR_MENU_WIDTH, CLEAR_MENU_HEIGHT);
		}
		// テクスチャの設定
		CPolygon::SetTexture(g_pTextures[nCntClearMenu+1]);
		// ポリゴンの描画
		CPolygon::Draw(pDeviceContext);
	}

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

//=============================================================================
// クリアメニューの設定
//=============================================================================
void CClear::SetClearMenu(void)
{
	m_fCurve = 0.0f;
}

//=============================================================================
// クリアメニューの取得
//=============================================================================
CLEAR_MENU CClear::GetClearMenu(void)
{
	return g_nSelectCMenu;
}

//=============================================================================
// クリアメニューのリセット
//=============================================================================
void CClear::ResetClearMenu(void)
{
	g_nSelectCMenu = CLEAR_MENU_SELECT;
	SetClearMenu();
}
//=============================================================================
// クリア画面表示フラグ
// @param	[bool]	Clearflg	クリアしたかどうか
// @return	[bool]	g_bClear	クリア情報
//=============================================================================
bool CClear::Clearflg(bool Clearflg)
{
	g_bClear = Clearflg;
	return g_bClear;
}
