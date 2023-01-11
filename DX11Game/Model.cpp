//=============================================================================
//
// ���f�� �N���X���� [Model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Model.h"
#include "Scene.h"

// �O���[�o���ϐ�
namespace {
	// 3D���f�� �t�@�C����
	LPCSTR g_pszModelPath[MAX_MODEL] = {
		"data/model/Player.fbx",
		"data/model/SkyDome/sky.fbx",
		"data/model/Land.fbx",
		"data/model/hover.fbx",
		"data/model/book.fbx",
		"data/model/city.fbx",
	};
}

// �ÓI�����o
CAssimpModel* CModel::m_pModels[MAX_MODEL] = { nullptr };

// �R���X�g���N�^
CModel::CModel(CScene* pScene) : CGameObj(pScene)
{
	m_pModel = nullptr;

	m_pVertex = nullptr;
	m_nVertex = 0;
	m_pIndex = nullptr;
	m_nIndex = 0;
}

// �f�X�g���N�^
CModel::~CModel()
{
}

// �S���f���Ǎ�
HRESULT CModel::LoadAll()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// Assimp�p�V�F�[�_������
	if (!CAssimpModel::InitShader(pDevice, 0))
		return E_FAIL;

	// �S���f���Ǎ�
	HRESULT hr = S_OK;
	for (int i = 0; i < MAX_MODEL; ++i) {
		SAFE_DELETE(m_pModels[i]);
		m_pModels[i] = new CAssimpModel();
		if (!m_pModels[i]->Load(pDevice, pDC, g_pszModelPath[i])) {
			hr = E_FAIL;
			break;
		}
	}
	return hr;
}

// �S���f�����
void CModel::ReleaseAll()
{
	// �S���f�����
	for (int i = 0; i < MAX_MODEL; ++i) {
		if (m_pModels[i]) {
			m_pModels[i]->Release();
			delete m_pModels[i];
			m_pModels[i] = nullptr;
		}
	}

	// Assimp�p�V�F�[�_�I������
	CAssimpModel::UninitShader();
}

// ������
HRESULT CModel::Init()
{
	HRESULT hr = CGameObj::Init();
	if (FAILED(hr))
		return hr;

	return hr;
}

// �I������
void CModel::Fin()
{
	CGameObj::Fin();
}

// �X�V
void CModel::Update()
{

	CGameObj::Update();
}

// �`��
void CModel::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	if (m_pModel) {
		m_pModel->Draw(pDC, GetWorld(), eOpacityOnly);
	}
}

// �����������`��
void CModel::DrawAlpha()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	if (m_pModel) {
		m_pModel->Draw(pDC, GetWorld(), eTransparentOnly);
	}
	CGameObj::DrawAlpha();
}

// ���f���ݒ�
void CModel::SetModel(EModel model)
{
	if (model < 0 || model >= MAX_MODEL) {
		m_pModel = nullptr;
		SetRadius(0.5f);
		SetBBox(XMFLOAT3(0.5f, 0.5f, 0.5f));
		SetCenter(XMFLOAT3(0, 0, 0));
	}
	else {
		m_pModel = m_pModels[model];
		SetRadius(m_pModel->GetRadius());
		SetBBox(m_pModel->GetBBox());
		SetCenter(m_pModel->GetCenter());
	}
}

// Assimp���f���擾
CAssimpModel* CModel::GetAssimp(EModel model)
{
	if (model < 0 || model >= MAX_MODEL)
		return nullptr;
	return m_pModels[model];
}

// ���_/�C���f�b�N�X�z�񒊏o
void CModel::InitVertexArray(EModel model)
{
	FinVertexArray();
	CAssimpModel* pModel = GetAssimp(model);
	if (!pModel) return;
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	m_pVertex = new TAssimpVertex[m_nVertex];
	m_pIndex = new UINT[m_nIndex];
	pModel->GetVertex(m_pVertex, m_pIndex);
}

// ���_/�C���f�b�N�X�z����
void CModel::FinVertexArray()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}

// ���C�Ƃ̓����蔻��
bool CModel::CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (UINT i = 0; i < m_nIndex; ) {
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
		XMStoreFloat(&dot, XMVector3Dot(n,
			XMVector3Cross(v2v0,
				XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f) {
			continue;
		}
		// �_�������������̂œ������Ă���
		if (pX) {
			*pX = vX;
		}
		if (pN) {
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// �������Ă��Ȃ�
}

// �����Ƃ̓����蔻��
bool CModel::CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// �S�Ă̎O�p�`�ɂ��ČJ��Ԃ�
	for (UINT i = 0; i < m_nIndex; ) {
		// �O�p�`��3���_
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2�ӂ��@���x�N�g�������߂�
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// �@���x�N�g���Ɛ����̕����x�N�g���Ƃ̓���(���̕���)�����߂�
		XMFLOAT3 vW;
		vW.x = vP1.x - vP0.x;
		vW.y = vP1.y - vP0.y;
		vW.z = vP1.z - vP0.z;
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
		// t�����Ȃ��_�͐����̌��Ȃ̂Ŏ��̎O�p�`��
		if (t < 0.0f) {
			continue;
		}
		// t��1���傫���ꍇ�͐����̐�Ȃ̂Ŏ��̎O�p�`��
		if (t > 1.0f) {
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
		XMStoreFloat(&dot, XMVector3Dot(n,
			XMVector3Cross(v2v0,
				XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f) {
			continue;
		}
		// �_�������������̂œ������Ă���
		if (pX) {
			*pX = vX;
		}
		if (pN) {
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// �������Ă��Ȃ�
}
