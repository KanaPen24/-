//=============================================================================
//
// ゲーム クラス定義 [Game.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameCamera.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Object.h"

//定数定義
#define MAX_OBJECT 10 

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
