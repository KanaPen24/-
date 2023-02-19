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
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "StegeSelect.h"


//定数定義
const int MAX_OBJECT = 30;


class CEnemy;
class CRadar;
class CScore;
class CPause;
// クラス定義
class CGame : public CScene
{
private:
	bool m_bResult;

	CGameCamera m_camera;
	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;
	CObject* m_pObject[MAX_OBJECT];
	CRadar* m_pRadar;
	CScore* m_pScore;
	CPause* m_pPause;
	CSSelect* m_pSSelect;

	
	int m_nBoundary;
	bool m_bPause;	//一時停止中
	bool m_bSoundPause;	//停止中の効果音一回流すよう
	float m_fAlf;

public:
	CGame();
	virtual ~CGame();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	int GetBoundary() { return m_nBoundary; }

	CPlayer* GetPlayer() { return m_pPlayer; }

	int m_nCntObj;

private:
	void UpdateBoundary();
};
