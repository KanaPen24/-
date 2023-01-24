/*=====�w�b�_�R�����g=====
*@�v���O������	�FScene.cpp
*@�����E�T�v	�F�V�[���̑匳�ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F�Q�[���I�[�o�[�ǉ�
*@�X�V����	�F2021/12/11
*/
#include "Scene.h"
#include "SceneTitle.h"
#include "Game.h"
#include "sceneSelect.h"
#include "sceneRule.h"
#include "GameObj.h"
#include "SceneClear.h"
#include "SceneGameover.h"

// �ÓI�����o
CScene* CScene::m_pScene = nullptr;
CScene* CScene::m_pTop = nullptr;

// �R���X�g���N�^
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

// �f�X�g���N�^
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

// ������
bool CScene::Init()
{
	CCamera::Set();
	return true;
}

// �I������
void CScene::Fin()
{
}

// ������
bool CScene::InitAll()
{
	new CTitle;
	new CGame;
	new CSelect;
	new CRule;
	new CClear;
	new CGOver;

	//�ŏ��̃V�[��
	Change(SCENE_GAME);
	return true;
}

// �I������
void CScene::FinAll()
{
	Change(SCENE_NONE);

	CScene* pNext;
	for (CScene* pScene = m_pTop; pScene; pScene = pNext) {
		pNext = pScene->m_pNext;
		delete pScene;
	}
}

// �X�V
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

// �`��
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

// �ύX
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

// �I�u�W�F�N�g����
CGameObj* CScene::Find(int id, CGameObj* pStart)
{
	CGameObj* pObj = (pStart) ? pStart->GetNext() : m_pObj;
	for (; pObj; pObj = pObj->GetNext()) {
		if (pObj->GetID() == id)
			return pObj;
	}
	return nullptr;
}
