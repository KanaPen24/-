/*=====�w�b�_�R�����g=====
*@�v���O������	�FStageSelect.cpp
*@�����E�T�v	�F�X�e�[�W�Z���N�g����
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

// �|�[�Y���j���[
typedef enum {
	STAGE_1 = 0,		// �X�e�[�W1
	STAGE_2,			// �X�e�[�W2
	STAGE_3,			// �X�e�[�W3
	STAGE_MAX
} STAGE_SELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSSelect(void);
void UninitSSelect(void);
void UpdateSSelect(void);
void DrawSSelect(void);

void SetStageSelect(void);
STAGE_SELECT GetStageSelect(void);
void ResetStageSelect(void);
