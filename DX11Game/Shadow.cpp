///*=====ヘッダコメント=====
//*@プログラム名	：shadow.cpp
//*@説明・概要	：影について
//*@製作者	：吉田叶聖
//*@更新内容	：2022/06/04
//*@更新履歴	：コメント追記
//*@更新者	：吉田叶聖
//*///*****************************************************************************
//// インクルード部
////*****************************************************************************
//#include "shadow.h"
//#include "Mesh.h"
//#include "Texture.h"
//#include "Light.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//const	TEXTURE_FILENAME	L"data/texture/shadow000.jpg"	// テクスチャファイル名
//
//const M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
//const M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
//const M_POWER				(50.0f)
//const M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
//const M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
//
//const MAX_SHADOW			5000
//
////*****************************************************************************
//// 構造体
////*****************************************************************************
//typedef struct {
//	int			stat;	// 状態
//	XMFLOAT3	pos;	// 位置
//	float		radius;	// 半径
//} SHADOW;
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
//static CMesh						g_mesh;					// 構造体
//static CMeshMaterial				g_material;				// マテリアル
//static SHADOW						g_shadow[MAX_SHADOW];	// 丸影情報
//
//// 初期化
//HRESULT InitShadow(void)
//{
//	ID3D11Device* pDevice = GetDevice();
//	HRESULT hr = S_OK;
//
//	// マテリアルの初期設定
//	g_material.m_Diffuse = M_DIFFUSE;
//	g_material.m_Ambient = M_AMBIENT;
//	g_material.m_Specular = M_SPECULAR;
//	g_material.m_Power = M_POWER;
//	g_material.m_Emissive = M_EMISSIVE;
//	g_mesh.SetMaterial(&g_material);
//
//	// テクスチャの読み込み
//	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, g_pTexture);
//	if (FAILED(hr))
//		return hr;
//	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());
//
//	
//	// 頂点情報の作成
//	g_mesh.m_nNumVertex = 4;
//	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.m_nNumVertex];
//	pVertexWk[0].vtx = XMFLOAT3(-0.5f, 0.0f,  0.5f);
//	pVertexWk[1].vtx = XMFLOAT3( 0.5f, 0.0f,  0.5f);
//	pVertexWk[2].vtx = XMFLOAT3(-0.5f, 0.0f, -0.5f);
//	pVertexWk[3].vtx = XMFLOAT3( 0.5f, 0.0f, -0.5f);
//	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	pVertexWk[0].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	pVertexWk[1].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	pVertexWk[2].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	pVertexWk[3].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
//	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
//	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
//	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);
//	g_mesh.m_nNumIndex = 4;
//	int* pIndexWk = new int[4];
//	pIndexWk[0] = 0;
//	pIndexWk[1] = 1;
//	pIndexWk[2] = 2;
//	pIndexWk[3] = 3;
//	hr = CMesh::MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);
//	delete[] pIndexWk;
//	delete[] pVertexWk;
//
//	return hr;
//}
//
//// 終了処理
//void UninitShadow(void)
//{
//	CMesh::FinShader();
//}
//
//// 更新
//void UpdateShadow(void)
//{
//	// 何もしない
//}
//
//// 描画
//void DrawShadow(void)
//{
//	ID3D11DeviceContext* pDC = GetDeviceContext();
//	CLight::Get()->SetDisable();
//	SetBlendState(BS_SUBTRACTION);	// 減算合成
//	SetZWrite(false);	// 半透明描画はZバッファを更新しない(Zチェックは行う)
//	SHADOW* pShadow = g_shadow;
//	for (int i = 0; i < MAX_SHADOW; ++i, ++pShadow) {
//		if (!pShadow->stat) continue;
//		float ratio = pShadow->pos.y / 320.0f;
//		if (ratio < 0.0f) ratio = 0.0f;
//		if (ratio > 1.0f) ratio = 1.0f;
//		
//		// 不透明度を反映
//		g_material.m_Diffuse.w = 0.5f - ratio;
//		if (g_material.m_Diffuse.w < 0.0f)
//			g_material.m_Diffuse.w = 0.0f;
//		// 描画
//		CMesh::Draw();
//	}
//	SetZWrite(true);
//	SetBlendState(BS_NONE);
//	SetBlendState(BS_ALPHABLEND);
//	CLight::Get()->SetEnable();
//}
//
//// 生成
//int CreateShadow(XMFLOAT3 pos, float radius)
//{
//	SHADOW* pShadow = g_shadow;
//	for (int i = 0; i < MAX_SHADOW; ++i, ++pShadow) {
//		if (pShadow->stat) continue;
//		pShadow->pos = pos;
//		pShadow->radius = radius;
//		pShadow->stat = 1;
//		return i;
//	}
//	return -1;
//}
//
//// 移動
//void MoveShadow(int nShadow, XMFLOAT3 pos)
//{
//	if (nShadow < 0 || nShadow >= MAX_SHADOW)
//		return;
//	g_shadow[nShadow].pos = pos;
//}
//
//// 解放
//void ReleaseShadow(int nShadow)
//{
//	if (nShadow < 0 || nShadow >= MAX_SHADOW)
//		return;
//	g_shadow[nShadow].stat = 0;
//}
