/*=====�w�b�_�R�����g=====
*@�v���O������	�FScore.h
*@�����E�T�v	�F�X�R�A�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"

class CScore
{
private:
	int m_nScore;
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Add(int nScore) { m_nScore += nScore; }
	int Get() {	return m_nScore;}
};