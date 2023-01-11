// 地面 [Land.h]
#pragma once
#include "Model.h"
#include "Scene.h"

class CLand : public CModel
{
private:
	UINT m_nVertex;				// 頂点数
	TAssimpVertex* m_pVertex;	// 頂点配列
	UINT m_nIndex;				// インデックス数
	UINT* m_pIndex;				// インデックス配列
	ID3D11ShaderResourceView*	m_pTexture;	// テクスチャ
	EScene m_pScene;			// 現在シーン
public:
	CLand(CScene* pScene);		// コンストラクタ
	virtual ~CLand();			// デストラクタ

	virtual HRESULT Init();		// 初期化
	virtual void Fin();			// 終了処理
	virtual void Draw();		// 描画

	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
								// レイとの当たり判定
};
