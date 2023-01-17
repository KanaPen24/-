/*=====�w�b�_�R�����g=====
*@�v���O������	�FTitleSelect.cpp
*@�����E�T�v	�F�^�C�g���Z���N�g����
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"

// �|�[�Y���j���[
typedef enum {
	GAME = 0,		// �Q�[���X�^�[�g
	END,			// �I��
	TITLE_MAX
} TITLE_SELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTSelect(void);
void UninitTSelect(void);
void UpdateTSelect(void);
void DrawTSelect(void);

void SetTitleSelect(void);
TITLE_SELECT GetTitleSelect(void);
void ResetTitleSelect(void);
