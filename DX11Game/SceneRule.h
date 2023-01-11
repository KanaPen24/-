/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneRule.h
*@�����E�T�v	�F���[���V�[���ɂ���
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
class CRule : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CRule();
	virtual ~CRule();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
