/*=====�w�b�_�R�����g=====
*@�v���O������	�FPostProcess.h
*@�����E�T�v	�F�|�X�g�v���Z�X�ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2023/02/02
*/
#pragma once
#include "Main.h"

// �N���X��`
class CPostProcess {
private:
	static XMFLOAT4 m_vColor;
	static float m_fRate;

public:
	static void Init();
	static void Update();
	static void Draw();
	static void SetColor(XMFLOAT3 vColor);
	static void SetAlf(float fAlf) { m_vColor.w = fAlf; }
};
