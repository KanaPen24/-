/*=====ヘッダコメント=====
*@プログラム名	：SceneSelect.h
*@説明・概要	：ステージセレクトについて
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
class CSelect : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;
	int m_nBlink;
	int m_nStart;

	bool bSound1;	//一回だけ流す
	bool bSound2;	//一回だけ流す
	bool bSound3;	//一回だけ流す

public:
	CSelect();
	virtual ~CSelect();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
