/*=====ヘッダコメント=====
*@プログラム名	：Box.h
*@説明・概要	：箱について
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
#pragma once
#include "Mesh.h"

class CBox : public CMesh
{
public:
	CBox();
	virtual ~CBox();

	HRESULT Init(XMFLOAT3 vBBox);
};
