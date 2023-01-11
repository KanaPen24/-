/*=====ヘッダコメント=====
*@プログラム名	：object.h
*@説明・概要	：オブジェクトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"
#include "Model.h"
#include "Land.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

class CPlayer;
class CObject : public CModel
{
private:
	CPlayer* m_pPlayer;
	CLand* m_pLand;			// 地面

public:
	CObject(CScene* pScene);
	virtual ~CObject();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	static int GetCnt();
};
