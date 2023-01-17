/*=====�w�b�_�R�����g=====
*@�v���O������	�FSceneGameover.h
*@�����E�T�v	�F�Q�[���I�[�o�[��ʂɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Main.h"
#include "Scene.h"
// �֐��v���g�^�C�v
// �N���X��`
class CGOver : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CGOver();
	virtual ~CGOver();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};