/*=====ヘッダコメント=====
*@プログラム名	：ScenePrologue.h
*@説明・概要	：プロローグシーンについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"
#include "Scene.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// クラス定義
class CPrologue : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CPrologue();
	virtual ~CPrologue();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
