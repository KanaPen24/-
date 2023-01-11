//=============================================================================
//
// ゲーム カメラ クラス [GameCamera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CTitleCamera : public CCamera
{
private:
	CGameObj* m_pCity;	// 街

	float m_fRadius;		// 自機半径
	int m_nMode;			// カメラモード
	float m_fAngle;			// カメラアングル

public:
	CTitleCamera();

	virtual void Init();
	virtual void Update();

	void SetCity(CGameObj* pCity);
};
