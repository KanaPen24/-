/*=====�w�b�_�R�����g=====
*@�v���O������	�FSphere.h
*@�����E�T�v	�F���̂ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Mesh.h"

class CSphere : public CMesh
{
public:
	CSphere();
	virtual ~CSphere();

	HRESULT Init(int nSlice = 16, int nStack = 8, float fRadius = 0.5f);
};
