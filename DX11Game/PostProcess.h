/*=====ヘッダコメント=====
*@プログラム名	：PostProcess.h
*@説明・概要	：ポストプロセスについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2023/02/02
*/
#pragma once
#include "Main.h"

// クラス定義
class CPostProcess {
private:
	static XMFLOAT4 m_vColor;
	static float m_fRate;

public:
	static void Init();
	static void Update();
	static void Draw();
	static void SetColor(XMFLOAT3 vColor);
	static void SetAlf(float fAlf) { m_vColor.w = fAlf; }
};
