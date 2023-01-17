/*=====ヘッダコメント=====
*@プログラム名	：GameCamera.h
*@説明・概要	：ゲームカメラについて
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CGameCamera : public CCamera
{
private:
	CGameObj* m_pPlayer;	// 自機

	float m_fRadius;		// 自機半径
	int m_nMode;			// カメラモード
	float m_fAngle;			// カメラアングル

public:
	CGameCamera();

	virtual void Init();
	virtual void Update();

	void SetPlayer(CGameObj* pPlayer);
};
