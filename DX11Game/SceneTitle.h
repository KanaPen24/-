/*=====�w�b�_�R�����g=====
*@�v���O������	�FSceneTitle.h
*@�����E�T�v	�F�^�C�g���V�[���ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/
#pragma once
#include "Scene.h"
#include "TitleCamera.h"
#include "City.h"

// �N���X��`
class CTitle : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	CTitleCamera m_camera;
	CCity* m_pCity;
	int m_nTimer;

public:
	CTitle();
	virtual ~CTitle();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
