/*=====�w�b�_�R�����g=====
*@�v���O������	�FScenePrologue.h
*@�����E�T�v	�F�v�����[�O�V�[���ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"
#include "Scene.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �N���X��`
class CPrologue : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CPrologue();
	virtual ~CPrologue();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
