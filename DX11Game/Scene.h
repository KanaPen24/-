/*=====ヘッダコメント=====
*@プログラム名	：Scene.h
*@説明・概要	：シーンの大元について
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：ゲームオーバー追加
*@更新履歴	：2021/12/11
*/
#pragma once
#include "Main.h"

// 定数定義
enum EScene {
	SCENE_NONE = 0, // 未設定
	SCENE_TITLE,	// タイトル画面
	SCENE_RULE,		//チュートリアル画面
	SCENE_SELECT,	// ステージセレクト画面
	SCENE_GAME,	// ステージ
	SCENE_CLEAR,	//クリア画面
	SCENE_GAMEOVER,	//ゲームオーバー画面
	MAX_SCENE
};

// クラス定義
class CGameObj;
class CScene {
protected:
	EScene m_id;
	CGameObj* m_pObj;

private:
	static CScene* m_pScene;
	static CScene* m_pTop;
	CScene* m_pBack;
	CScene* m_pNext;

public:
	CScene();
	virtual ~CScene();

	virtual bool Init();
	virtual void Fin();
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static bool InitAll();
	static void FinAll();
	static void UpdateAll();
	static void DrawAll();

	static void Change(EScene scene);

	CGameObj* GetObj() { return m_pObj; }
	void SetObj(CGameObj* pObj) { m_pObj = pObj; }
	EScene GetScene() { return m_id; }
	CGameObj* Find(int id, CGameObj* pStart = nullptr);
};
