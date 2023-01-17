/*=====ヘッダコメント=====
*@プログラム名	：Sphere.h
*@説明・概要	：球体について
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
#pragma once
#include "Mesh.h"

class CSphere : public CMesh
{
public:
	CSphere();
	virtual ~CSphere();

	HRESULT Init(int nSlice = 16, int nStack = 8, float fRadius = 0.5f);
};
