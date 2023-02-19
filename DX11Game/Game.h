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
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "StegeSelect.h"


//�萔��`
const int MAX_OBJECT = 30;


class CEnemy;
class CRadar;
class CScore;
class CPause;
// �N���X��`
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
	bool m_bPause;	//�ꎞ��~��
	bool m_bSoundPause;	//��~���̌��ʉ���񗬂��悤
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
