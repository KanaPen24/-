/*=====�w�b�_�R�����g=====
*@�v���O������	�FStaminabar.h
*@�����E�T�v	�F�X�^�~�i�o�[�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/03/06
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "main.h"
#include "Game.h"

enum {
	ST_STAMINABAR,		// �X�^�~�i�o�[

	MAX_ST
};

class CGame;
class CStaminaBar
{
private:
	
	static ID3D11ShaderResourceView* m_pTexture[MAX_ST];
	bool m_bVisible;
	CGame* m_pScene;
	CPlayer* m_pPlayer;
	float m_fStamina;

public:
	void Init(CGame* pScene);
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();
};
