/*=====ヘッダコメント=====
*@プログラム名	：Particle.h
*@説明・概要	：パーティクルについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/10
*@更新履歴	：評価課題で作成
*@更新者	：吉田叶聖
*/
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int nLife);
XMFLOAT3 GetParticlePos();	// 発生位置取得
