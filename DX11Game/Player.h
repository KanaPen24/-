//=============================================================================
//
// プレイヤー クラス定義 [Player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Model.h"
#include "Land.h"

class CPlayer : public CModel
{
private:
	UINT m_uTick;
	XMFLOAT4X4 m_mInvWorld;
	int m_nSpeed;			// 速さ
	CLand* m_pLand;			// 地面
	int m_animNo;			// アニメーションNo.
	double m_animTime;		// 再生時間
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	XMFLOAT4X4& GetInvWorld() { return m_mInvWorld; }
};
