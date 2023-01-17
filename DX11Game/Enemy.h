/*=====ヘッダコメント=====
*@プログラム名	：Enemy.h
*@説明・概要	：敵の挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/24
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "Model.h"
#include "Land.h"

class CPlayer;
class CEnemy : public CModel
{
private:
	CPlayer* m_pPlayer;
	CLand* m_pLand;			// 地面

public:
	CEnemy(CScene* pScene);
	virtual ~CEnemy();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};
