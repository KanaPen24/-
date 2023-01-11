//=============================================================================
//
// �^�C�g����ʃN���X��` [Title.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
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
