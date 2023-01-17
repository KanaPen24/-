/*=====�w�b�_�R�����g=====
*@�v���O������	�FTitleCamera.h
*@�����E�T�v	�F�^�C�g���J�����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CTitleCamera : public CCamera
{
private:
	CGameObj* m_pCity;	// �X

	float m_fRadius;		// ���@���a
	int m_nMode;			// �J�������[�h
	float m_fAngle;			// �J�����A���O��

public:
	CTitleCamera();

	virtual void Init();
	virtual void Update();

	void SetCity(CGameObj* pCity);
};
