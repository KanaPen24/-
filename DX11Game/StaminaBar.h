/*=====ヘッダコメント=====
*@プログラム名	：Staminabar.h
*@説明・概要	：スタミナバーについて
*@製作者	：吉田叶聖
*@更新内容	：2023/03/06
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "main.h"
#include "Game.h"

enum {
	ST_STAMINABAR,		// スタミナバー

	MAX_ST
};

class CGame;
class CStaminaBar
{
private:
	
	static ID3D11ShaderResourceView* m_pTexture[MAX_ST];
	bool m_bVisible;
	CGame* m_pScene;
	CPlayer* m_pPlayer;
	float m_fStamina;

public:
	void Init(CGame* pScene);
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();
};
