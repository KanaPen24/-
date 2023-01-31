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
#include "Game.h"

class CObject;
class CScore
{
private:
	CObject* m_pObj;
	CGame* m_pScene;
	
public:
	HRESULT Init(CGame* pScene);
	void Uninit();
	void Update();
	void Draw();
	void Add(int nScore);
	int m_nScore;
};