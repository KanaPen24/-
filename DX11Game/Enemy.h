/*=====�w�b�_�R�����g=====
*@�v���O������	�FEnemy.h
*@�����E�T�v	�F�G�̋����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/24
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#pragma once
#include "Model.h"
#include "Land.h"

class CPlayer;
class CEnemy : public CModel
{
private:
	CPlayer* m_pPlayer;
	CLand* m_pLand;			// �n��

public:
	CEnemy(CScene* pScene);
	virtual ~CEnemy();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};
