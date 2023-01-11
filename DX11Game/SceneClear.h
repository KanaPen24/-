/*=====�w�b�_�R�����g=====
*@�v���O������	�Fclear.h
*@�����E�T�v	�F�|�[�Y��ʂȂǂɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/12/20
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"
#include "Scene.h"

// �|�[�Y���j���[
typedef enum {
	CLEAR_MENU_SELECT=0,	// �X�e�[�W�Z���N�g
	CLEAR_MENU_RETRY,		// ���g���C
	CLEAR_MENU_TITLE,
	CLEAR_MENU_MAX
} CLEAR_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �N���X��`
class CClear : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CClear();
	virtual ~CClear();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetClearMenu(void);
	CLEAR_MENU GetClearMenu(void);
	void ResetClearMenu(void);
	static bool Clearflg(bool Clearflg);
};
