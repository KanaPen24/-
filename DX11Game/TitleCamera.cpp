//=============================================================================
//
// �Q�[�� �J���� �N���X [GameCamera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "TitleCamera.h"
#include "Model.h"
#include "Input.h"
#include "Debugproc.h"


// �O���[�o���ϐ�
namespace {
	const float CAM_POS_P_X = -200.0f;	// ��_�J�����̎��_�ʒu(X���W)
	const float CAM_POS_P_Y = 500.0f;	// ��_�J�����̎��_�ʒu(Y���W)
	const float CAM_POS_P_Z = -550.0f;	// ��_�J�����̎��_�ʒu(Z���W)
	const float CAM_POS_R_X = -200.0f;	// ��_�J�����̒����_�ʒu(X���W)
	const float CAM_POS_R_Y = 300.0f;	// ��_�J�����̒����_�ʒu(Y���W)
	const float CAM_POS_R_Z = 0.0f;		// ��_�J�����̒����_�ʒu(Z���W)

	const float FP_POS_P_X = 0.0f;		// ��l�̃J�����̎��_�ʒu(X���W)
	const float FP_POS_P_Y = 50.6f;		// ��l�̃J�����̎��_�ʒu(Y���W)
	const float FP_POS_P_Z = 100.0f;	// ��l�̃J�����̎��_�ʒu(Z���W)
	const float FP_POS_R_X = 0.0f;		// ��l�̃J�����̒����_�ʒu(X���W)
	const float FP_POS_R_Y = 22.6f;		// ��l�̃J�����̒����_�ʒu(Y���W)
	const float FP_POS_R_Z = 259.96f;	// ��l�̃J�����̒����_�ʒu(Z���W)

	const float TP_POS_P_X = 0.0f;		// �O�l�̃J�����̎��_�ʒu(X���W)
	const float TP_POS_P_Y = 100.0f;	// �O�l�̃J�����̎��_�ʒu(Y���W)
	const float TP_POS_P_Z = 300.0f;	// �O�l�̃J�����̎��_�ʒu(Z���W)
	const float TP_POS_R_X = 0.0f;		// �O�l�̃J�����̒����_�ʒu(X���W)
	const float TP_POS_R_Y = 80.0f;		// �O�l�̃J�����̒����_�ʒu(Y���W)
	const float TP_POS_R_Z = 0.0f;		// �O�l�̃J�����̒����_�ʒu(Z���W)
};

// �J�������[�h
enum ECamMode {
	CM_FIXED = 0,	// �Œ�
	CM_FP,			// ��l��
	CM_BT,			// �o���b�g�^�C����
	CM_TP,			// �O�l��

	MAX_CM
};

// �R���X�g���N�^
CTitleCamera::CTitleCamera()
{
	m_pCity = nullptr;
	m_fAngle = 0.0f;
	m_fRadius = 1.0f;
	m_nMode = CM_FIXED;
}

// ������
void CTitleCamera::Init()
{
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// �X�V
void CTitleCamera::Update()
{
	XMFLOAT3 pPos = m_pCity->GetPos();		//�X�̍��W�擾

	// ���_�ؑ�
	if (CInput::GetKeyRelease(VK_0) || CInput::GetKeyRelease(VK_NUMPAD0))
		m_nMode = 0;
	if (CInput::GetKeyRelease(VK_1) || CInput::GetKeyRelease(VK_NUMPAD1))
		m_nMode = 1;
	if (CInput::GetKeyRelease(VK_2) || CInput::GetKeyRelease(VK_NUMPAD2))
		m_nMode = 2;
	if (CInput::GetKeyRelease(VK_3) || CInput::GetKeyRelease(VK_NUMPAD3))
		m_nMode = 3;

	if (m_pCity) {
		//XMMATRIX mW;
		switch (m_nMode) {
		case CM_FIXED:	// ��_�J����
			m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);;
			m_vPos = XMFLOAT3(CAM_POS_P_X + pPos.x, CAM_POS_P_Y + pPos.y, CAM_POS_P_Z + pPos.z);
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}
	} else {
		m_vPos = XMFLOAT3(CAM_POS_P_X + pPos.x, CAM_POS_P_Y + pPos.y, CAM_POS_P_Z + pPos.z);
		m_vTarget = m_pCity->GetPos();
		m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	CCamera::Update();
#ifdef _DEBUG
	// �f�o�b�O�\��
	CDebugProc::Print("*** ��� ��� ***\n");
	CDebugProc::Print("[1] : FP View\n");
	CDebugProc::Print("[2] : Bullet-time style\n");
	CDebugProc::Print("[3] : TP View\n");
	CDebugProc::Print("[0] : Fixed point\n");
	CDebugProc::Print("[��� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[������ : (%f, %f, %f)]\n",
		m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("\n");
#endif
}

// ���@�ݒ�
void CTitleCamera::SetCity(CGameObj* pCity)
{
	m_pCity = pCity;
	if (m_pCity) {
		m_fRadius = m_pCity->GetRadius();
	} else {
		m_fRadius = 1.0f;
	}
}
