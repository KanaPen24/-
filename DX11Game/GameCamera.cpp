/*=====�w�b�_�R�����g=====
*@�v���O������	�FGameCamera.cpp
*@�����E�T�v	�F�Q�[���J�����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#include "GameCamera.h"
#include "Model.h"
#include "Input.h"
#include "Debugproc.h"
#include "Player.h"

// �O���[�o���ϐ�
namespace {
	const float CAM_POS_P_X = 0.0f;		// ��_�J�����̎��_�ʒu(X���W)
	const float CAM_POS_P_Y = 70.0f;	// ��_�J�����̎��_�ʒu(Y���W)
	const float CAM_POS_P_Z = -200.0f;	// ��_�J�����̎��_�ʒu(Z���W)
	const float CAM_POS_R_X = 0.0f;		// ��_�J�����̒����_�ʒu(X���W)
	const float CAM_POS_R_Y = 0.0f;		// ��_�J�����̒����_�ʒu(Y���W)
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

	const float DISTANCE = 9.0f;		// �J�����̋���
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
CGameCamera::CGameCamera()
{
	m_pPlayer = nullptr;
	m_fAngle = 0.0f;
	m_fRadius = 1.0f;
	m_nMode = CM_FIXED;
}

// ������
void CGameCamera::Init()
{
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// �X�V
void CGameCamera::Update()
{
	XMFLOAT3 pPos = m_pPlayer->GetPos();		//�v���C���[�̍��W�擾
	
	if(CInput::GetKeyPress(VK_J))
	m_fAngle -= 1.0f;
	if (CInput::GetKeyPress(VK_L))
	m_fAngle += 1.0f;
	if (m_fAngle >= 180.0f)m_fAngle -= 360.0f;	
	m_vTarget = m_pPlayer->GetPos();
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vPos.x = m_vTarget.x + sinf(XMConvertToRadians(m_fAngle))*(m_fRadius*DISTANCE);
	m_vPos.y = CAM_POS_P_Y;
	m_vPos.z = m_vTarget.z - cosf(XMConvertToRadians(m_fAngle))*(m_fRadius*DISTANCE);
	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	CCamera::Update();
#ifdef _DEBUG
	// �f�o�b�O�\��
	CDebugProc::Print("*** ��� ��� ***\n");
	CDebugProc::Print("[��� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[������ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("[��� : (%f)]\n", m_fLengthInterval);
	CDebugProc::Print("\n");
#endif
}

// ���@�ݒ�
void CGameCamera::SetPlayer(CGameObj* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer) {
		m_fRadius = m_pPlayer->GetRadius();
	} else {
		m_fRadius = 1.0f;
	}
}
