// 地面 [City.h]
#pragma once
#include "Model.h"

class CCity : public CModel
{
private:
	UINT m_nVertex;				// 頂点数
	TAssimpVertex* m_pVertex;	// 頂点配列
	UINT m_nIndex;				// インデックス数
	UINT* m_pIndex;				// インデックス配列
	ID3D11ShaderResourceView*	m_pTexture;	// テクスチャ
	XMFLOAT3 a;
public:
	CCity(CScene* pScene);		// コンストラクタ
	virtual ~CCity();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Draw();		// 描画
	
	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW,bool collision, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
								// レイとの当たり判定
};
