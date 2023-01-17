/*=====ヘッダコメント=====
*@プログラム名	：Model.h
*@説明・概要	：モデルについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/17
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

enum EModel {
	MODEL_PLAYER = 0,	// プレイヤー モデル
	MODEL_SKY,			// スカイドーム
	MODEL_LAND,			// 地面モデル
	MODEL_ENEMY,		// 敵モデル
	MODEL_OBJECT,		//オブジェクトモデル
	MODEL_CITY,			//街モデル

	MAX_MODEL
};

class CModel : public CGameObj
{
private:
	static CAssimpModel* m_pModels[MAX_MODEL];
	CAssimpModel* m_pModel;

	UINT m_nVertex;				// 頂点数
	TAssimpVertex* m_pVertex;	// 頂点配列
	UINT m_nIndex;				// インデックス数
	UINT* m_pIndex;				// インデックス配列

public:
	CModel(CScene* pScene);
	virtual ~CModel();

	static HRESULT LoadAll();
	static void ReleaseAll();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	void SetModel(EModel model);
	static CAssimpModel* GetAssimp(EModel model);
	CAssimpModel* GetModel() { return m_pModel; }

	void InitVertexArray(EModel model);
	void FinVertexArray();

	// レイとの当たり判定
	bool CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);

	// 線分との当たり判定
	bool CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
};
