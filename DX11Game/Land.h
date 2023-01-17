/*=====�w�b�_�R�����g=====
*@�v���O������	�FLand.h
*@�����E�T�v	�F�n�ʂɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Model.h"
#include "Scene.h"

class CLand : public CModel
{
private:
	UINT m_nVertex;				// ���_��
	TAssimpVertex* m_pVertex;	// ���_�z��
	UINT m_nIndex;				// �C���f�b�N�X��
	UINT* m_pIndex;				// �C���f�b�N�X�z��
	ID3D11ShaderResourceView*	m_pTexture;	// �e�N�X�`��
	EScene m_pScene;			// ���݃V�[��
public:
	CLand(CScene* pScene);		// �R���X�g���N�^
	virtual ~CLand();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Draw();		// �`��

	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
								// ���C�Ƃ̓����蔻��
};
