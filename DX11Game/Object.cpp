/*=====�w�b�_�R�����g=====
*@�v���O������	�FObject.cpp
*@�����E�T�v	�F�������悤�A�C�e���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Game.h"
#include "Sound.h"
#include "Score.h"
#include "Polygon.h"
#include "Debugproc.h"
#include "Scene.h"
#include "StegeSelect.h"

// �O���[�o���ϐ�
namespace {
	static int g_cntObject;
	static int g_MaxObject;
}

// �R���X�g���N�^
CObject::CObject(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_OBJECT;
	m_pLand = nullptr;

	m_pPlayer = nullptr;
	m_vPos = GetPos();
	m_vAngle = GetAngle();
}

// �f�X�g���N�^
CObject::~CObject()
{
}

// ������
HRESULT CObject::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_OBJECT);
	SetScale(XMFLOAT3(200.0f, 200.0f, 200.0f));
	m_pLand = (CLand*)m_pScene->Find(GOT_LAND);
	m_pScore = new CScore;
	switch (GetStageSelect())
	{
	case STAGE_1:
		g_MaxObject = 10;
		break;
	case STAGE_2:
		g_MaxObject = 20;
		break;
	case STAGE_3:
		g_MaxObject = 30;
		break;
	}
	g_cntObject = g_MaxObject;
	return hr;
}

// �I������
void CObject::Fin()
{
	CModel::Fin();
}

// �X�V
void CObject::Update()
{
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
	// �Փ˔���
	if (CollisionBSphere(m_pPlayer)) {
		m_pScore->Add(-1);
		m_vPos.x = 50000.0f;
		CSound::SetVolume(SE_DAMAGE, 0.2f);
		CSound::Play(SE_DAMAGE);
	}

	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;
	SetPos(m_vPos);

#ifdef _DEBUG
	CDebugProc::Print("[�� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
#endif
}

//�`��
void CObject::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 0);
	CModel::Draw();
}


int CObject::GetCnt()
{
	return g_cntObject;
}