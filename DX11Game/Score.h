/*=====ヘッダコメント=====
*@プログラム名	：Score.h
*@説明・概要	：スコアについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"
#include "Game.h"

class CObject;
class CScore
{
private:
	CObject* m_pObj;
	CGame* m_pScene;
	
public:
	HRESULT Init(CGame* pScene);
	void Uninit();
	void Update();
	void Draw();
	void Add(int nScore);
	int m_nScore;
};