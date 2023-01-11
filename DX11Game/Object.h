/*=====�w�b�_�R�����g=====
*@�v���O������	�Fobject.h
*@�����E�T�v	�F�I�u�W�F�N�g�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"
#include "Model.h"
#include "Land.h"
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class CPlayer;
class CObject : public CModel
{
private:
	CPlayer* m_pPlayer;
	CLand* m_pLand;			// �n��

public:
	CObject(CScene* pScene);
	virtual ~CObject();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	static int GetCnt();
};
