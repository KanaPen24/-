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
	const float CAM_POS_P_Y = 70.0f;	// 定点カメラの視点位置(Y座標)
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

	const float DISTANCE = 9.0f;		// カメラの距離
	const float ROTATE = 1.7f;		// カメラの距離
	const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	// 左スティックのデッドゾーン

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
	m_fRadius = 1.0f;
	m_nMode = CM_FIXED;
}

// 初期化
void CGameCamera::Init()
{
	CCamera::Init();
	m_fAngle = 0.0f;
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// 更新
void CGameCamera::Update()
{
	XMFLOAT3 pPos = m_pPlayer->GetPos();		//プレイヤーの座標取得
	//ゲームコントローラー
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	if (JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE)
	{
		JoyX = 0;
	}
	if (JoyCount >= 1) {
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			m_fAngle += ROTATE;
		}
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE) {
			m_fAngle -= ROTATE;
		}
	}
	if(CInput::GetKeyPress(VK_J))
	m_fAngle -= ROTATE;
	if (CInput::GetKeyPress(VK_L))
	m_fAngle += ROTATE;
	if (m_fAngle >= 180.0f)m_fAngle -= 360.0f;	
	m_vTarget = m_pPlayer->GetPos();
	m_vTarget.y += 30.0f;
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vPos.x = m_vTarget.x + sinf(XMConvertToRadians(m_fAngle))*(m_fRadius*DISTANCE);
	m_vPos.y = CAM_POS_P_Y;
	m_vPos.z = m_vTarget.z - cosf(XMConvertToRadians(m_fAngle))*(m_fRadius*DISTANCE);

	//カメラからターゲットの距離	
	m_vVec.x = m_vPos.x - m_vTarget.x;
	m_vVec.z = m_vPos.z - m_vTarget.z;

	CCamera::Update();
#ifdef _DEBUG
	// デバッグ表示
	CDebugProc::Print("*** ｶﾒﾗ ｿｳｻ ***\n");
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
