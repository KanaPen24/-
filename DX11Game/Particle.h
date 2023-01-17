/*=====�w�b�_�R�����g=====
*@�v���O������	�FParticle.h
*@�����E�T�v	�F�p�[�e�B�N���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/10
*@�X�V����	�F�]���ۑ�ō쐬
*@�X�V��	�F�g�c����
*/
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int nLife);
XMFLOAT3 GetParticlePos();	// �����ʒu�擾
