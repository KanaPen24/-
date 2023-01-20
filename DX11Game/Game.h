/*=====ヘッダコメント=====
*@プログラム名	：Game.h
*@説明・概要	：ゲームシーンについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/17
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameCamera.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Object.h"

//定数定義
const int MAX_OBJECT = 1;

// クラス定義
class CGame : public CScene
{
private:
	bool m_bResult;

	CGameCamera m_camera;
	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;
	CObject* m_pObject[MAX_OBJECT];

	int m_nBoundary;

public:
	CGame();
	virtual ~CGame();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int GetBoundary() { return m_nBoundary; }

	CPlayer* GetPlayer() { return m_pPlayer; }

private:
	void UpdateBoundary();
};
