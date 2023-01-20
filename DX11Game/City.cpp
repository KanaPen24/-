/*=====�w�b�_�R�����g=====
*@�v���O������	�FCity.cpp
*@�����E�T�v	�F�X�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#include "city.h"
#include "Shader.h"
#include "Texture.h"

const XMFLOAT3 POS = XMFLOAT3(0.0f, -40.0f, 0.0f);

// �R���X�g���N�^
CCity::CCity(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_CITY;

	m_pVertex = nullptr;
	m_nVertex = 0;
	m_pIndex = nullptr;
	m_nIndex = 0;
}

// �f�X�g���N�^
CCity::~CCity()
{
}

// ������
HRESULT CCity::Init()
{
	// ���[���h�}�g���b�N�X����
	XMFLOAT4X4 mW;
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	SetModel(MODEL_CITY);
	// ���_�z��A�C���f�b�N�X�z����擾���Ă���
	CAssimpModel* pModel = GetAssimp(MODEL_CITY);
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	m_pVertex = new TAssimpVertex[m_nVertex];
	m_pIndex = new UINT[m_nIndex];
	pModel->GetVertex(m_pVertex, m_pIndex);
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	SetAngle(XMFLOAT3(0.0f, 0.0f, 0.0f));
	/*mW._41 = POS.x;
	mW._42 = POS.y;
	mW._43 = POS.x;*/
	SetPos(POS);
	SetWorld(mW);

	return CModel::Init();
	
}

// �I������
void CCity::Fin()
{
	CModel::Fin();

	// ���_�z��A�C���f�b�N�X�z��̉��
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}

// ���C�Ƃ̓����蔻��
bool CCity::Collision(XMFLOAT3 vP0, XMFLOAT3 vW, bool collision, XMFLOAT3* pX, XMFLOAT3* pN)
{
	//���[���h���W�ϊ����܂������ĂȂ������C����Ȃ��Đ����ł̓����蔻��
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (int i = 0; i < m_nIndex; ) {
		// �O�p�`��3���_
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2�ӂ��@���x�N�g�������߂�
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// �@���x�N�g���ƃ��C�̕����x�N�g���Ƃ̓���(���̕���)�����߂�
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// ���ꂪ0�Ȃ�ʂƕ��s�̂��ߎ��̎O�p�`��
		if (base == 0.0f) {
			continue;
		}
		// �}��ϐ�t�����߂�
		float t;
		XMStoreFloat(&t, XMVector3Dot(n,
			XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// t�����Ȃ��_�̓��C�̌��Ȃ̂Ŏ��̎O�p�`��
		if (t < 0.0f) {
			continue;
		}
		// ��_X�����߂�
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// ��_���O�p�`�̓��������ׂ�
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
			XMVector3Cross(v0v1,
				XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f) {
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
			XMVector3Cross(v1v2,
				XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f) {
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n, XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f) {
			continue;
		}
		// �_�������������̂œ������Ă���
		if (pX && collision) {
			*pX = vX;
			a = vX;
		}
		else
		{
			*pX = a;
		}
		if (pN) {
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// �������Ă��Ȃ�
}

void CCity::Draw()
{
	// �@���}�b�v�ݒ�
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 0);
	CModel::Draw();
}
