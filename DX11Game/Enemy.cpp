/*=====�w�b�_�R�����g=====
*@�v���O������	�FEnemy.cpp
*@�����E�T�v	�F�G�̋����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/24
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "Debugproc.h"
#include "Fade.h"
#include "Sound.h"
#include "Input.h"
#include "PostProcess.h"

const float VALUE_MOVE_ENEMY = 1.0f;		// �ړ���
const float RATE_ROTATE_ENEMY = 0.20f;		// ��]�����W��
const float VALUE_ROTATE_ENEMY = 7.0f;		// ��]���x
const float SCALE = 10.0f;		// �傫��
const float MAX_VOLUME = 1.0f;	// �ő剹��
const float MIN_VOLUME = 0.15f;	// �ŏ�����
const float LEFT_VOLUME = -1.0f;	// ������
const float RIGHT_VOLUME = 1.0f;	// �E����
const float DELALFA = 0.15f;	// �A���t�@�l�����炷

//�O���[�o���ϐ�
bool g_bAlflg = false;

// �R���X�g���N�^
CEnemy::CEnemy(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_ENEMY;
	m_pPlayer = nullptr;
	m_fChan = 0.0f;
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
}

// ������
HRESULT CEnemy::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_ENEMY);
	SetScale(XMFLOAT3(SCALE, SCALE, SCALE));
	SetPos(XMFLOAT3(-500.0f + (float)(rand() % 500), 0.0f, 400.0f + (float)(rand() % 1000)));
	
	return hr;
}

// �I������
void CEnemy::Fin()
{
	CModel::Fin();
}

// �X�V
void CEnemy::Update()
{
	//�f�o�b�N�p�����I���I�t
	if (CInput::GetKeyTrigger(VK_1))
		g_bAlflg = true;
	if (CInput::GetKeyTrigger(VK_2))
		g_bAlflg = false;

	XMFLOAT3 pPos = m_pPlayer->GetPos();		//�v���C���[�̍��W�擾
	// ���݈ʒu�擾
	XMFLOAT3 vPos = GetPos();
	XMFLOAT3 vAng = m_pPlayer->GetAngle();		//�v���C���[�̊p�x�擾
	// ��]
	XMFLOAT3 angle = GetAngle();

	// �ʒu�𒊏o�A0�N���A
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	m_mWorld._41 = 0.0f;
	m_mWorld._42 = 0.0f;
	m_mWorld._43 = 0.0f;
	// ���[�J��X���AZ���𒊏o
	XMFLOAT3 vX(m_mWorld._11, m_mWorld._12, m_mWorld._13);
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// ��]
	XMMATRIX mRotate = XMMatrixIdentity();
	bool bRotate = false;
	// �ړI�̊p�x�܂ł̍���
	//float fDiffRotY = g_EnemyRotDest.y - g_EnemyRot.y;

	// �v���C���[��ǔ�����
	if (pPos.x  > vPos.x + 5.0f) {
		vPos.x += VALUE_MOVE_ENEMY;
		if (90 <= vAng.y &&vAng.y <= 275)
		m_fChan -= 0.01f;	//L�̉���
		else
		m_fChan += 0.01f;	//R�̉���
	}
	else if (pPos.x  < vPos.x - 5.0f)
	{
		vPos.x += -VALUE_MOVE_ENEMY;
		if (90 <= vAng.y &&vAng.y <= 275)
		m_fChan += 0.01f;	//R�̉���
		else
		m_fChan -= 0.01f;	//L�̉���
	}
	else
	{
		if (m_fChan > 0.0f)
			m_fChan -= 0.01f;
		if (m_fChan < 0.0f)
			m_fChan += 0.01f;
		//x�����d�Ȃ�Ƃ��ɂԂ�Ԃ邷��̂�h������
		vPos.x = vPos.x;
	}
	if (pPos.z > vPos.z)
	{
		vPos.z += VALUE_MOVE_ENEMY;
	}
	else
	{
		vPos.z += -VALUE_MOVE_ENEMY;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	// �p�x�X�V
	SetAngle(angle);

	// �ړ�
	m_vPos.x = vPos.x;
	m_vPos.y = vPos.y;
	m_vPos.z = vPos.z;
	//�ʒu�𔽉f
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	SetPos(m_vPos);

	//�����ɂ���ăT�E���h����������
	float x, z, volume;
	x = m_vPos.x - pPos.x;
	z = m_vPos.z - pPos.z;
	volume = x * x + z * z;
	sqrt(volume);

	//�{�����[������
	volume = volume / 50000;
	volume = MAX_VOLUME - volume;

	//�{�����[���̍ő�l�ŏ��l�ݒ�
	volume = std::fmax(volume, MIN_VOLUME);
	volume = std::fmin(volume, MAX_VOLUME);

	//LR�̍ő�l�ŏ��l�ݒ�
	m_fChan= std::fmax(m_fChan, LEFT_VOLUME);
	m_fChan = std::fmin(m_fChan, RIGHT_VOLUME);

	//��ʂ̃A���t�@�l��volume(�G�Ƃ̋���)�����
	//1.0�܂ł����ƃQ�[����ʂ������Ȃ��Ȃ�̂�
	//�����l�����炷
	CPostProcess::SetAlf(volume - DELALFA);

	//�{�����[���ݒ�
	CSound::SetVolume(BGM_GAME, volume, m_fChan);

	// �Փ˔���
	if (CollisionBSphere(m_pPlayer)) {
		CFade::CutOut(SCENE_GAMEOVER);
		CSound::SetVolume(SE_DAMAGE, 0.2f);
		CSound::Play(SE_DAMAGE);
	}

#ifdef _DEBUG
	CDebugProc::Print("[÷ �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[Vol : (%f)\n", volume);
	CDebugProc::Print("[Chan : (%f)\n", m_fChan);
#endif
}

//�`��
void CEnemy::Draw()
{
	if (g_bAlflg) {
		ID3D11Device* pDevice = GetDevice();
		CAssimpModel::InitShader(pDevice, 0);
		CModel::Draw();
	}
}
