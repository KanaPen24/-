/*=====�w�b�_�R�����g=====
*@�v���O������	�FScene.h
*@�����E�T�v	�F�V�[���̑匳�ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F�Q�[���I�[�o�[�ǉ�
*@�X�V����	�F2021/12/11
*/
#pragma once
#include "Main.h"

// �萔��`
enum EScene {
	SCENE_NONE = 0, // ���ݒ�
	SCENE_TITLE,	// �^�C�g�����
	SCENE_RULE,		//�`���[�g���A�����
	SCENE_SELECT,	// �X�e�[�W�Z���N�g���
	SCENE_GAME,	// �X�e�[�W
	SCENE_CLEAR,	//�N���A���
	SCENE_GAMEOVER,	//�Q�[���I�[�o�[���
	MAX_SCENE
};

// �N���X��`
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
