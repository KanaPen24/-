/*=====ヘッダコメント=====
*@プログラム名	：StageSelect.cpp
*@説明・概要	：ステージセレクトついて
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

// ポーズメニュー
typedef enum {
	STAGE_1 = 0,		// ステージ1
	STAGE_2,			// ステージ2
	STAGE_3,			// ステージ3
	STAGE_MAX
} STAGE_SELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSSelect(void);
void UninitSSelect(void);
void UpdateSSelect(void);
void DrawSSelect(void);

void SetStageSelect(void);
STAGE_SELECT GetStageSelect(void);
void ResetStageSelect(void);
