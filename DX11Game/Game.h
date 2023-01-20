/*=====�w�b�_�R�����g=====
*@�v���O������	�FGame.h
*@�����E�T�v	�F�Q�[���V�[���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameCamera.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Object.h"

//�萔��`
const int MAX_OBJECT = 1;

// �N���X��`
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
