/*=====�w�b�_�R�����g=====
*@�v���O������	�FBox.h
*@�����E�T�v	�F���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F�R�����g�ǋL
*@�X�V����	�F2023/01/17
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Mesh.h"

class CBox : public CMesh
{
public:
	CBox();
	virtual ~CBox();

	HRESULT Init(XMFLOAT3 vBBox);
};
