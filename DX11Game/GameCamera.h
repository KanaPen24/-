/*=====�w�b�_�R�����g=====
*@�v���O������	�FGameCamera.h
*@�����E�T�v	�F�Q�[���J�����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CGameCamera : public CCamera
{
private:
	CGameObj* m_pPlayer;	// ���@

	float m_fRadius;		// ���@���a
	int m_nMode;			// �J�������[�h
	float m_fAngle;			// �J�����A���O��

public:
	CGameCamera();

	virtual void Init();
	virtual void Update();

	void SetPlayer(CGameObj* pPlayer);
};
