/*=====�w�b�_�R�����g=====
*@�v���O������	�FObject.h
*@�����E�T�v	�F�������悤�A�C�e���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Main.h"
#include "Model.h"
#include "Land.h"
#include "Player.h"
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class CPlayer;
class CScore;
class CObject : public CModel
{
private:
	CPlayer* m_pPlayer;
	CCity* m_pCity;			// �X
	CScore* m_pScore;

public:
	CObject(CScene* pScene);
	virtual ~CObject();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};
