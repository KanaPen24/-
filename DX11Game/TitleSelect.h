/*=====ヘッダコメント=====
*@プログラム名	：TitleSelect.cpp
*@説明・概要	：タイトルセレクトついて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"

// ポーズメニュー
typedef enum {
	GAME = 0,		// ゲームスタート
	END,			// 終了
	TITLE_MAX
} TITLE_SELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTSelect(void);
void UninitTSelect(void);
void UpdateTSelect(void);
void DrawTSelect(void);

void SetTitleSelect(void);
TITLE_SELECT GetTitleSelect(void);
void ResetTitleSelect(void);
