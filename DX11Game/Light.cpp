/*=====�w�b�_�R�����g=====
*@�v���O������	�FLight.cpp
*@�����E�T�v	�F�����⃉�C�g�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Light.h"

using namespace DirectX;

// �O���[�o���ϐ�
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, 1.0f, 0.0f);		//����
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);	//�g�U
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f);//����
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);	//���ʔ���

	CLight g_light;
};

// �ÓI�����o�ϐ�
CLight* CLight::m_pLight = &g_light;

// �R���X�g���N�^
CLight::CLight()
{
	Init();
}

// ������
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	m_bEnable = true;
}

// ���������擾
XMFLOAT3& CLight::GetDir()
{
	//�V�[���ŋ�����ς���
	if (m_pScene == SCENE_TITLE)
	{
		static float q;
		q += 0.00005f;
		m_direction = XMFLOAT3(sin(q), -1.0f, cos(q));
	}
	else
	{
		m_direction = XMFLOAT3(0.0f, -1.0f, 1.0f);
	}
	if (m_bEnable) return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

// �����C���X�^���X�擾
CLight* CLight::Get()
{
	return m_pLight;
}

// �����C���X�^���X�ݒ�
void CLight::Set(CLight* pLight)
{
	m_pLight = (pLight) ? pLight : &g_light;
}
