/*=====ヘッダコメント=====
*@プログラム名	：GameCamera.cpp
*@説明・概要	：ゲームカメラについて
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
#include "GameCamera.h"
#include "Model.h"
#include "Input.h"
#include "Debugproc.h"
#include "Player.h"

// グローバル変数
namespace {
	const float CAM_POS_P_X = 0.0f;		// 定点カメラの視点位置(X座標)
	const float CAM_POS_P_Y = 90.0f;	// 定点カメラの視点位置(Y座標)
	const float CAM_POS_P_Z = -200.0f;	// 定点カメラの視点位置(Z座標)
	const float CAM_POS_R_X = 0.0f;		// 定点カメラの注視点位置(X座標)
	const float CAM_POS_R_Y = 0.0f;		// 定点カメラの注視点位置(Y座標)
	const float CAM_POS_R_Z = 0.0f;		// 定点カメラの注視点位置(Z座標)

	const float FP_POS_P_X = 0.0f;		// 一人称カメラの視点位置(X座標)
	const float FP_POS_P_Y = 50.6f;		// 一人称カメラの視点位置(Y座標)
	const float FP_POS_P_Z = 100.0f;	// 一人称カメラの視点位置(Z座標)
	const float FP_POS_R_X = 0.0f;		// 一人称カメラの注視点位置(X座標)
	const float FP_POS_R_Y = 22.6f;		// 一人称カメラの注視点位置(Y座標)
	const float FP_POS_R_Z = 259.96f;	// 一人称カメラの注視点位置(Z座標)

	const float TP_POS_P_X = 0.0f;		// 三人称カメラの視点位置(X座標)
	const float TP_POS_P_Y = 100.0f;	// 三人称カメラの視点位置(Y座標)
	const float TP_POS_P_Z = 300.0f;	// 三人称カメラの視点位置(Z座標)
	const float TP_POS_R_X = 0.0f;		// 三人称カメラの注視点位置(X座標)
	const float TP_POS_R_Y = 80.0f;		// 三人称カメラの注視点位置(Y座標)
	const float TP_POS_R_Z = 0.0f;		// 三人称カメラの注視点位置(Z座標)
};

// カメラモード
enum ECamMode {
	CM_FIXED = 0,	// 固定
	CM_FP,			// 一人称
	CM_BT,			// バレットタイム風
	CM_TP,			// 三人称

	MAX_CM
};

// コンストラクタ
CGameCamera::CGameCamera()
{
	m_pPlayer = nullptr;
	m_fAngle = 0.0f;
	m_fRadius = 1.0f;
	m_nMode = CM_FIXED;
}

// 初期化
void CGameCamera::Init()
{
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// 更新
void CGameCamera::Update()
{
	XMFLOAT3 pPos = m_pPlayer->GetPos();		//プレイヤーの座標取得

	// 視点切替
	if (CInput::GetKeyRelease(VK_0) || CInput::GetKeyRelease(VK_NUMPAD0))
		m_nMode = 0;
	
	if (m_pPlayer) {
		//XMMATRIX mW;
		switch (m_nMode) {
		case CM_FIXED:	// 定点カメラ
			m_vTarget = m_pPlayer->GetPos();
			m_vPos = XMFLOAT3(CAM_POS_P_X + pPos.x, CAM_POS_P_Y + pPos.y, CAM_POS_P_Z + pPos.z);
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}
	} else {
		m_vPos = XMFLOAT3(CAM_POS_P_X + pPos.x, CAM_POS_P_Y + pPos.y, CAM_POS_P_Z + pPos.z);
		m_vTarget = m_pPlayer->GetPos();
		m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	CCamera::Update();
#ifdef _DEBUG
	// デバッグ表示
	CDebugProc::Print("*** ｶﾒﾗ ｿｳｻ ***\n");
	CDebugProc::Print("[1] : FP View\n");
	CDebugProc::Print("[2] : Bullet-time style\n");
	CDebugProc::Print("[3] : TP View\n");
	CDebugProc::Print("[0] : Fixed point\n");
	CDebugProc::Print("[ｶﾒﾗ ｲﾁ : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[ﾁｭｳｼﾃﾝ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("\n");
#endif
}

// 自機設定
void CGameCamera::SetPlayer(CGameObj* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer) {
		m_fRadius = m_pPlayer->GetRadius();
	} else {
		m_fRadius = 1.0f;
	}
}
