//=============================================================================
//
// �G�N���X���� [Enemy.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "Debugproc.h"
#include "Fade.h"
#include "Sound.h"

const float VALUE_MOVE_ENEMY = 1.0f;		// �ړ���
const float RATE_ROTATE_ENEMY = 0.20f;		// ��]�����W��
const float VALUE_ROTATE_ENEMY = 7.0f;		// ��]���x
const float SCALE = 10.0f;		// �傫��
const float MAX_VOLUME = 1.0f;	// �ő剹��

// �R���X�g���N�^
CEnemy::CEnemy(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_ENEMY;
	m_pLand = nullptr;

	m_pPlayer = nullptr;
	
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
	m_pPlayer = (CPlayer*)m_pScene->Find(GOT_PLAYER);
	m_pLand = (CLand*)m_pScene->Find(GOT_LAND);
	SetScale(XMFLOAT3(SCALE, SCALE, SCALE));
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

	float fChan = 0.0f;	//�T�E���h��LR�����p

	XMFLOAT3 pPos = m_pPlayer->GetPos();		//�v���C���[�̍��W�擾
	// ���݈ʒu�擾
	XMFLOAT3 vPos = GetPos();
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
	if (pPos.x > vPos.x) {
		vPos.x = VALUE_MOVE_ENEMY;
		fChan = -1.0f;
	}
	else if (pPos.x < vPos.x)
	{
		vPos.x = -VALUE_MOVE_ENEMY;
		fChan = 1.0f;
	}
	else
	{
		//x�����d�Ȃ�Ƃ��ɂԂ�Ԃ邷��̂�h������
		fChan = 0.0f;
		vPos.x = vPos.x;
	}
	if (pPos.y > vPos.y) {
		vPos.y = VALUE_MOVE_ENEMY;
	}
	else
	{
		vPos.y = -VALUE_MOVE_ENEMY;
	}
	if (pPos.z > vPos.z)
	{
		vPos.z = VALUE_MOVE_ENEMY;
	}
	else
	{
		vPos.z = -VALUE_MOVE_ENEMY;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	// �p�x�X�V
	SetAngle(angle);

	// �ړ�
	m_vPos.x += vPos.x;
	m_vPos.y += vPos.y;
	m_vPos.z += vPos.z;
	//�ʒu�𔽉f
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	// �n�ʂƂ̓����蔻��
	XMFLOAT3 vP0;
	vP0.x = m_vPos.x;
	vP0.y = m_vPos.y + GetRadius() * 2;
	vP0.z = m_vPos.z;
	XMFLOAT3 vX2, vN;
	if (m_pLand && m_pLand->Collision(vP0,
		XMFLOAT3(0.0f, -1.0f, 0.0f), &vX2, &vN)) {
		m_vPos.y = vX2.y;
	}
	else {
		m_vPos.y = 0.0f;
	}

	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;
	SetPos(m_vPos);

	//�����̂���ăT�E���h����������
	float x, z, volume;
	x = vPos.x - pPos.x;
	z = vPos.z - pPos.z;
	volume = x * x + z * z;
	sqrt(volume);
	volume = volume / 50000;
	volume = MAX_VOLUME - volume;
	volume = std::max(volume, 0.15f);
	volume = std::min(volume, 0.8f);
	CSound::SetVolume(BGM_GAME, volume, fChan);

	// �Փ˔���
	if (m_pPlayer) {
		switch (((CGame*)m_pScene)->GetBoundary()) {
		case 0:
			break;
		case 1:
			if (CollisionBSphere(m_pPlayer)) {
				m_pPlayer->SetBColor();
				CFade::Out(SCENE_GAMEOVER);
				//CSound::SetVolume(SE_DAMAGE, 0.2f);
				//CSound::Play(SE_DAMAGE);
				SetBColor();
			}
			break;
		}
	}
#ifdef _DEBUG
	CDebugProc::Print("[÷ �� : (%f, %f, %f)]\n", vPos.x, vPos.y, vPos.z);
#endif
}

//�`��
void CEnemy::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 0);
	CModel::Draw();
}
