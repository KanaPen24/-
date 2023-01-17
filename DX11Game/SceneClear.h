/*=====ヘッダコメント=====
*@プログラム名	：SceneClear.h
*@説明・概要	：クリア画面画面などについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/12/20
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"
#include "Scene.h"

// ポーズメニュー
typedef enum {
	CLEAR_MENU_SELECT=0,	// ステージセレクト
	CLEAR_MENU_RETRY,		// リトライ
	CLEAR_MENU_TITLE,
	CLEAR_MENU_MAX
} CLEAR_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// クラス定義
class CClear : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CClear();
	virtual ~CClear();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetClearMenu(void);
	CLEAR_MENU GetClearMenu(void);
	void ResetClearMenu(void);
	static bool Clearflg(bool Clearflg);
};
