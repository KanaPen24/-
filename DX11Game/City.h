// �n�� [City.h]
#pragma once
#include "Model.h"

class CCity : public CModel
{
private:
	UINT m_nVertex;				// ���_��
	TAssimpVertex* m_pVertex;	// ���_�z��
	UINT m_nIndex;				// �C���f�b�N�X��
	UINT* m_pIndex;				// �C���f�b�N�X�z��
	ID3D11ShaderResourceView*	m_pTexture;	// �e�N�X�`��
	XMFLOAT3 a;
public:
	CCity(CScene* pScene);		// �R���X�g���N�^
	virtual ~CCity();			// �f�X�g���N�^

	virtual HRESULT Init();		// ������
	virtual void Fin();			// �I������
	virtual void Draw();		// �`��
	
	bool Collision(XMFLOAT3 vP0, XMFLOAT3 vW,bool collision, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
								// ���C�Ƃ̓����蔻��
};
