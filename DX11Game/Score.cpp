/*=====ヘッダコメント=====
*@プログラム名	：score.cpp
*@説明・概要	：スコア表示について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*///*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Score.h"
#include "Number.h"
#include "Texture.h"
#include "Polygon.h"
#include "Object.h"
#include "Debugproc.h"
#include "Fade.h"
#include "Scene.h"
#include "SceneClear.h"

// マクロ定義
const int CHRCNT_SCORE = 2;
const int POS_X_SCORE = 0.0f;
const float POS_Y_SCORE = ((SCREEN_HEIGHT - 80.0f) * 0.5f + 16.0f);
const int MAX_SCORE = 1;
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;

// 初期化
HRESULT CScore::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// 変数初期化
	m_nScore = MAX_SCORE;
	return hr;
}
// 終了処理
void CScore::Uninit()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
// 更新
void CScore::Update()
{
	if (m_nScore == 0)
	{
//		CFade::CutOut(SCENE_CLEAR);
		CClear::Clearflg(true);
	}
}
// 描画
void CScore::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// 明るい緑
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	// スコア表示
	DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)m_nScore, CHRCNT_SCORE);
	// 元に戻す
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}
