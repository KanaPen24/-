/*=====ヘッダコメント=====
*@プログラム名	：PostProcess.cpp
*@説明・概要	：ポストプロセスついて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2023/02/02
*/
#include "PostProcess.h"
#include "Polygon.h"
#include "Sound.h"
#include "Input.h"
#include "DebugProc.h"

// 静的メンバ
XMFLOAT4 CPostProcess::m_vColor = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);
float CPostProcess::m_fRate = 1.0f / (60.0f * 0.5f);

// 初期化
void CPostProcess::Init()
{
	m_vColor.w = 0.0f;			// 不透明度
}

// 更新
void CPostProcess::Update()
{
	if(CInput::GetKeyPress(VK_B))
	m_vColor.w += m_fRate;	// 不透明度を増加
	if (CInput::GetKeyPress(VK_M))
	m_vColor.w -= m_fRate;	// 不透明度を下げる

	//最大値と最小値を設定
	m_vColor.w = std::fmax(m_vColor.w, 0.0f);
	m_vColor.w = std::fmin(m_vColor.w, 0.5f);

#ifdef _DEBUG
	CDebugProc::Print("Alf%2f\n", m_vColor.w);
#endif // _DEBUG



	
}

// 描画
void CPostProcess::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(m_vColor);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// フェードイン アウト カラー設定
void CPostProcess::SetColor(XMFLOAT3 vColor)
{
	m_vColor.x = vColor.x;
	m_vColor.y = vColor.y;
	m_vColor.z = vColor.z;
}

