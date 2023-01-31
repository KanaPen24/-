// レーダー [Radar.h]
#pragma once
#include "main.h"
#include "Game.h"

enum {
	RT_RADAR,		// レーダー
	RT_BRIGHTSPOT,	// 輝点

	MAX_RT
};

class CRadar
{
private:
	static ID3D11ShaderResourceView* m_pTexture[MAX_RT];

	bool m_bVisible;
	XMFLOAT3 m_vPos[MAX_OBJECT];
	XMFLOAT3 m_vPos2D[MAX_OBJECT];
	CGame* m_pScene;
	int m_nObject;
	int m_nCntObj;
	CPlayer* m_pPlayer;

public:
	void Init(CGame* pScene);
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();
};
