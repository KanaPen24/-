/*=====ヘッダコメント=====
*@プログラム名	：StageSelect.cpp
*@説明・概要	：ステージセレクトついて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#include "StegeSelect.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const int NUM_SELECT_MENU = 3;					// セレクトメニュー数
const float SELECT_MENU_WIDTH = 360.0f;			// セレクトメニュー幅
const float SELECT_MENU_HEIGHT = 360.0f;		// セレクトメニュー高さ
const float SELECT_MENU_POS_X = -420.0f;		// セレクトメニュー位置(X座標)
const float SELECT_MENU_POS_Y = 0.0f;			// セレクトメニュー位置(Y座標)
const float SELECT_MENU_INTERVAL = 400.0f;		// セレクトメニュー間隔
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	//左スティックのデッドゾーン
const float UNDER_MOVE_RIMIT_Y = -10.f;			//ロゴの動く下の上限値
const float TOP_MOVE_RIMIT_Y = 10.f;			//ロゴの動く上の上限値
const float ROGO_SPEED = 0.5f;					//ロゴが動くスピード


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static STAGE_SELECT g_nSelectSMenu = STAGE_1;		// 選択中のメニューNo.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static bool g_bJoyStick;
static bool flg;
static float g_fPosY;

static LPCWSTR c_aFileNameStageMenu[NUM_SELECT_MENU] =
{
	L"data/texture/stage1.png",	// ステージ1
	L"data/texture/stage2.png",	// ステージ2
	L"data/texture/stage3.png",	// ステージ3
};

//=============================================================================
// ステージセレクト初期化処理
//=============================================================================
HRESULT InitSSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,									// デバイスへのポインタ
			c_aFileNameStageMenu[nCntSelectMenu],	// ファイルの名前
			&g_pTextures[nCntSelectMenu]);			// 読み込むメモリー
	}

	g_nSelectSMenu = STAGE_1;
	g_fCurve = 0.0f;
	g_bJoyStick = false;
	flg = true;
	g_fPosY = SELECT_MENU_POS_Y;


	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// ステージセレクト終了処理
//=============================================================================
void UninitSSelect(void)
{
	// テクスチャの開放
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SAFE_RELEASE(g_pTextures[nCntSelectMenu]);
	}
}

//=============================================================================
// ステージセレクト更新処理
//=============================================================================
void UpdateSSelect(void)
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0l;
	}
	if (JoyCount >= 1) {
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
				CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
				CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_LEFT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}
	if (CInput::GetKeyRepeat(VK_RIGHT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// 反射光の設定
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	//ロゴのふわふわ感
	if (flg) {
		g_fPosY = g_fPosY - ROGO_SPEED;
	}
	if (!flg) {
		g_fPosY = g_fPosY + ROGO_SPEED;
	}
	if (g_fPosY <= UNDER_MOVE_RIMIT_Y) flg = false;
	if (g_fPosY >= TOP_MOVE_RIMIT_Y)flg = true;


}

//=============================================================================
// ステージセレクト描画処理
//=============================================================================
void DrawSSelect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);

		if (nCntSelectMenu == g_nSelectSMenu) {
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, g_fPosY);
		}
		else {
			CPolygon::SetColor(0.0f, 0.0f, 0.0f, 0.5f);
			CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);
		}
		// テクスチャの設定
		CPolygon::SetTexture(g_pTextures[nCntSelectMenu]);
		// ポリゴンの描画
		CPolygon::Draw(pDeviceContext);
	}
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ステージセレクトメニューの設定
//=============================================================================
void SetStageSelect(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ステージセレクトメニューの取得
//=============================================================================
STAGE_SELECT GetStageSelect(void)
{
	return g_nSelectSMenu;
}

//=============================================================================
// ステージセレクトメニューのリセット
//=============================================================================
void ResetStageSelect(void)
{
	g_nSelectSMenu = STAGE_1;
	//CSound::Play(SE_SHIZUKU);
	SetStageSelect();
}
