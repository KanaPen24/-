/*=====�w�b�_�R�����g=====
*@�v���O������	�FSceneSelect.h
*@�����E�T�v	�F�X�e�[�W�Z���N�g�ɂ���
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
class CSelect : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;
	int m_nBlink;
	int m_nStart;

	bool bSound1;	//��񂾂�����
	bool bSound2;	//��񂾂�����
	bool bSound3;	//��񂾂�����

public:
	CSelect();
	virtual ~CSelect();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
