/*=====ヘッダコメント=====
*@プログラム名	：TitleCamera.h
*@説明・概要	：タイトルカメラについて
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
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
