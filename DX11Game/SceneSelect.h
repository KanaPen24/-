/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneSelect.h
*@�����E�T�v	�F�X�e�[�W�Z���N�g�ɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
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

public:
	CSelect();
	virtual ~CSelect();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
