//=============================================================================
//
// �G�N���X��` [Enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
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
