/*=====ヘッダコメント=====
*@プログラム名	：gameover.cpp
*@説明・概要	：ゲームオーバー画面について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "SceneGameover.h"
#include "Texture.h"
#include "Polygon.h"
#include "Input.h"
#include "Scene.h"
#include "Sound.h"
#include "Fade.h"
#include "Time.h"

// マクロ定義
#define PATH_BGTEXTURE L"data/texture/ゲームオーバーデッドエンド赤.png"

const float BG_POS_X = 0.0f;
const float BG_POS_Y = 0.0f;
const int BG_WIDTH = SCREEN_WIDTH;
const int BG_HEIGHT = SCREEN_HEIGHT;
const int GAMEOVER_TIMER = 250;
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
// コンストラクタ
CGOver::CGOver() : CScene()
{
	m_id = SCENE_GAMEOVER;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;

}

// デストラクタ
CGOver::~CGOver()
{
}

// 初期化
bool CGOver::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if(FAILED(hr)) {
		return hr;
	}
	// BGM再生
	//CSound::Play(SE_GAMEOVER);
	return hr;
}
// 終了処理
void CGOver::Fin() {
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);

}
// 更新
void CGOver::Update() {
	// [Enter]またはスペースキー押下
		// [Enter]またはスペースキー押下
	if (CInput::GetKeyRelease(VK_RETURN)){// || GetKeyRelease(VK_SPACE)||GetJoyButton(JOYSTICKID1,JOYBUTTON1)) {
		// タイトル画面へ
		CFade::Out(SCENE_SELECT);
		return;
	}
}
// 描画
void CGOver::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CPolygon::SetSize(BG_WIDTH, BG_HEIGHT);
	CPolygon::SetPos(BG_POS_X, BG_POS_Y);
	CPolygon::SetTexture(g_pTexture);
	CPolygon::Draw(pDC);
}