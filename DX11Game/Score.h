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

class CScore
{
private:
	int m_nScore;
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Add(int nScore) { m_nScore += nScore; }
	int Get() {	return m_nScore;}
};