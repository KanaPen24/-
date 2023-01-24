/*=====�w�b�_�R�����g=====
*@�v���O������	�FPlayer.h
*@�����E�T�v	�F�v���C���[�̃��f���y�ы����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Model.h"
#include "City.h"
#include "GameCamera.h"

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
	CCamera* m_pGCam;	// �Q�[���J����
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
