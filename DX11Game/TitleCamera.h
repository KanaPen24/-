//=============================================================================
//
// �Q�[�� �J���� �N���X [GameCamera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
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
