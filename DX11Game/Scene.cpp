/*=====ヘッダコメント=====
*@プログラム名	：Scene.cpp
*@説明・概要	：シーンの大元について
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：ゲームオーバー追加
*@更新履歴	：2021/12/11
*/
#include "Scene.h"
#include "SceneTitle.h"
#include "Game.h"
#include "sceneSelect.h"
#include "sceneRule.h"
#include "GameObj.h"
#include "SceneClear.h"
#include "SceneGameover.h"

// 静的メンバ
CScene* CScene::m_pScene = nullptr;
CScene* CScene::m_pTop = nullptr;

// コンストラクタ
CScene::CScene()
{
	m_id = SCENE_NONE;
	m_pObj = nullptr;

	m_pBack = nullptr;
	m_pNext = m_pTop;
	if (m_pTop) {
		m_pTop->m_pBack = this;
	}
	m_pTop = this;
}

// デストラクタ
CScene::~CScene()
{
	if (m_pNext) {
		m_pNext->m_pBack = m_pBack;
	}
	if (m_pBack) {
		m_pBack->m_pNext = m_pNext;
	} else {
		m_pTop = m_pNext;
	}
	m_pBack = m_pNext = nullptr;
}

// 初期化
bool CScene::Init()
{
	CCamera::Set();
	return true;
}

// 終了処理
void CScene::Fin()
{
}

// 初期化
bool CScene::InitAll()
{
	new CTitle;
	new CGame;
	new CSelect;
	new CRule;
	new CClear;
	new CGOver;

	//最初のシーン
	Change(SCENE_GAME);
	return true;
}

// 終了処理
void CScene::FinAll()
{
	Change(SCENE_NONE);

	CScene* pNext;
	for (CScene* pScene = m_pTop; pScene; pScene = pNext) {
		pNext = pScene->m_pNext;
		delete pScene;
	}
}

// 更新
void CScene::UpdateAll()
{
	if (m_pScene) {
		m_pScene->Update();
		CCamera* pCamera = CCamera::Get();
		if (pCamera) {
			pCamera->Update();
		}
	}
}

// 描画
void CScene::DrawAll()
{
	if (m_pScene) {
		CCamera* pCamera = CCamera::Get();
		if (pCamera) {
			pCamera->Clear();
		}
		m_pScene->Draw();
	}
}

// 変更
void CScene::Change(EScene scene)
{
	if (m_pScene) {
		m_pScene->Fin();
		m_pScene = nullptr;
	}
	if (scene) {
		for (CScene* pScene = m_pTop; pScene; pScene = pScene->m_pNext) {
			if (pScene->m_id == scene) {
				m_pScene = pScene;
				break;
			}
		}
		if (m_pScene) {
			m_pScene->Init();
		}
	}
}

// オブジェクト検索
CGameObj* CScene::Find(int id, CGameObj* pStart)
{
	CGameObj* pObj = (pStart) ? pStart->GetNext() : m_pObj;
	for (; pObj; pObj = pObj->GetNext()) {
		if (pObj->GetID() == id)
			return pObj;
	}
	return nullptr;
}
