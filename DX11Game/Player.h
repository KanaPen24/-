/*=====ヘッダコメント=====
*@プログラム名	：Player.h
*@説明・概要	：プレイヤーのモデル及び挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "Model.h"
#include "City.h"
#include "GameCamera.h"

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
	CCamera* m_pGCam;	// ゲームカメラ
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
