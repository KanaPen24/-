//=============================================================================
//
// プレイヤー クラス定義 [Player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Model.h"
#include "City.h"

class CPlayer : public CModel
{
private:
	UINT m_nVertex;				// 頂点数
	UINT m_nIndex;				// インデックス数

	UINT m_uTick;
	XMFLOAT4X4 m_mInvWorld;
	int m_nSpeed;			// 速さ
	CCity* m_pCity;			// 街
	int m_naAnimNo;			// アニメーションNo.
	double m_dAnimTime;		// 再生時間
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();
	bool m_bCollision;
	XMFLOAT4X4& GetInvWorld() { return m_mInvWorld; }
};
