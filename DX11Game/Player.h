//=============================================================================
//
// �v���C���[ �N���X��` [Player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Model.h"
#include "Land.h"

class CPlayer : public CModel
{
private:
	UINT m_uTick;
	XMFLOAT4X4 m_mInvWorld;
	int m_nSpeed;			// ����
	CLand* m_pLand;			// �n��
	int m_animNo;			// �A�j���[�V����No.
	double m_animTime;		// �Đ�����
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	XMFLOAT4X4& GetInvWorld() { return m_mInvWorld; }
};
