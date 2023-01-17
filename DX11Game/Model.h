/*=====�w�b�_�R�����g=====
*@�v���O������	�FModel.h
*@�����E�T�v	�F���f���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

enum EModel {
	MODEL_PLAYER = 0,	// �v���C���[ ���f��
	MODEL_SKY,			// �X�J�C�h�[��
	MODEL_LAND,			// �n�ʃ��f��
	MODEL_ENEMY,		// �G���f��
	MODEL_OBJECT,		//�I�u�W�F�N�g���f��
	MODEL_CITY,			//�X���f��

	MAX_MODEL
};

class CModel : public CGameObj
{
private:
	static CAssimpModel* m_pModels[MAX_MODEL];
	CAssimpModel* m_pModel;

	UINT m_nVertex;				// ���_��
	TAssimpVertex* m_pVertex;	// ���_�z��
	UINT m_nIndex;				// �C���f�b�N�X��
	UINT* m_pIndex;				// �C���f�b�N�X�z��

public:
	CModel(CScene* pScene);
	virtual ~CModel();

	static HRESULT LoadAll();
	static void ReleaseAll();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	void SetModel(EModel model);
	static CAssimpModel* GetAssimp(EModel model);
	CAssimpModel* GetModel() { return m_pModel; }

	void InitVertexArray(EModel model);
	void FinVertexArray();

	// ���C�Ƃ̓����蔻��
	bool CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);

	// �����Ƃ̓����蔻��
	bool CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
};
