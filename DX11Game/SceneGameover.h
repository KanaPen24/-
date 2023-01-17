/*=====ヘッダコメント=====
*@プログラム名	：SceneGameover.h
*@説明・概要	：ゲームオーバー画面について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "Main.h"
#include "Scene.h"
// 関数プロトタイプ
// クラス定義
class CGOver : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CGOver();
	virtual ~CGOver();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};