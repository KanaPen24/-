//=============================================================================
//
// �v���C���[ �N���X��` [Player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Model.h"
#include "City.h"

class CPlayer : public CModel
{
private:
	UINT m_nVertex;				// ���_��
	UINT m_nIndex;				// �C���f�b�N�X��

	UINT m_uTick;
	XMFLOAT4X4 m_mInvWorld;
	int m_nSpeed;			// ����
	CCity* m_pCity;			// �X
	int m_naAnimNo;			// �A�j���[�V����No.
	double m_dAnimTime;		// �Đ�����
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();
	bool m_bCollision;
	XMFLOAT4X4& GetInvWorld() { return m_mInvWorld; }
};
