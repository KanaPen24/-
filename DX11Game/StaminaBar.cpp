/*=====ヘッダコメント=====
*@プログラム名	：Staminabar.cpp
*@説明・概要	：スタミナバーについて
*@製作者	：吉田叶聖
*@更新内容	：2023/03/06
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "StaminaBar.h"
#include "Texture.h"
#include "Polygon.h"
#include "DebugProc.h"

// グローバル変数
namespace {
	LPCTSTR PATH_TEXTURE[MAX_ST] = {
		_T("data/texture/Bar.png"),
	};
	
	const XMFLOAT4 STAMINABAR_COLOR = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.8f);
	const XMFLOAT2 SIZE_XY = XMFLOAT2(50.0f, 50.0f);
	const float STAMINABAR_POS_X = SCREEN_WIDTH / -2;
	const float STAMINABAR_POS_Y = SCREEN_HEIGHT / -2 + (SIZE_XY.y / 2);
	
}

// 静的メンバ変数
ID3D11ShaderResourceView* CStaminaBar::m_pTexture[MAX_ST];

// 初期化
void CStaminaBar::Init(CGame* pScene)
{
	m_pScene = pScene;
	m_bVisible = true;
	m_pPlayer = m_pScene->GetPlayer();
	m_fStamina = m_pPlayer->GetStamina();
}

// 更新
void CStaminaBar::Update()
{
	if (!m_bVisible) return;

	//スタミナ増減受け取り
	m_fStamina = m_pPlayer->GetStamina();
	
#ifdef _DEBUG
	if (CInput::GetKeyPress(VK_RIGHT))
		m_fStamina++;
	if (CInput::GetKeyPress(VK_LEFT))
		m_fStamina--;
	
#endif // _DEBUG

}

// 描画
void CStaminaBar::Draw()
{
	if (!m_bVisible) return;

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);

	// スタミナバー設定
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetFrameSize(0.0f, 1.0f);
	CPolygon::SetColor(STAMINABAR_COLOR);
	CPolygon::SetTexture(m_pTexture[ST_STAMINABAR]);
	CPolygon::SetSize(m_fStamina*8, SIZE_XY.y);
	CPolygon::SetPos(STAMINABAR_POS_X + (m_fStamina / 2), -STAMINABAR_POS_Y);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	//値リセット
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// テクスチャ読込
HRESULT CStaminaBar::LoadTexture()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	ReleaseTexture();
	// テクスチャの読み込み
	for (int i = 0; i < MAX_ST; ++i) {
		hr = CreateTextureFromFile(pDevice, PATH_TEXTURE[i], &m_pTexture[i]);
		if (FAILED(hr)) {
			break;
		}
	}
	return hr;
}

// テクスチャ解放
void CStaminaBar::ReleaseTexture()
{
	for (int i = 0; i < MAX_ST; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
	}
}
