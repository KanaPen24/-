/*=====�w�b�_�R�����g=====
*@�v���O������	�FDebugProc.H
*@�����E�T�v	�F�f�o�b�N�����\���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/24
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Main.h"

class CDebugProc
{
private:
	static ID3D11ShaderResourceView* m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	static char m_szDebug[8192];					// �f�o�b�O���
	static bool m_bHiragana;						// �������t���O

public:
	static HRESULT Init();
	static void Fin();
	static void Draw();

	static void Start(bool hiragana = false);
	static int Print(const char *fmt, ...);
	static bool SetHiragana(bool hiragana = true);
};
