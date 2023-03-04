/*=====ヘッダコメント=====
*@プログラム名	：Fade.h
*@説明・概要	：シーン切り替え時フェードイン/アウトについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
#include "Main.h"
#include "Scene.h"

// マクロ定義
enum EFade {
	FADE_NONE = 0,	// 何もしていない
	FADE_IN,		// フェードイン処理
	FADE_OUT,		// フェードアウト処理

	MAX_FADE
};

// クラス定義
class CFade {
private:
	static EFade m_fade;
	static EScene m_sceneNext;
	static XMFLOAT4 m_vColor;
	static float m_fRate;

public:
	static void Init();
	static void Update();
	static void Draw();

	static void Out(EScene scene, float fSecond = 0.5f);
	static EFade Get() { return m_fade; }
	static void SetColor(XMFLOAT3 vColor);
	static void CutOut(EScene eNext);
};
