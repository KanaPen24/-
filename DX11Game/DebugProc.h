/*=====ヘッダコメント=====
*@プログラム名	：DebugProc.H
*@説明・概要	：デバック文字表示について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/24
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "Main.h"

class CDebugProc
{
private:
	static ID3D11ShaderResourceView* m_pTexture;	// テクスチャへのポインタ
	static char m_szDebug[8192];					// デバッグ情報
	static bool m_bHiragana;						// 平仮名フラグ

public:
	static HRESULT Init();
	static void Fin();
	static void Draw();

	static void Start(bool hiragana = false);
	static int Print(const char *fmt, ...);
	static bool SetHiragana(bool hiragana = true);
};
