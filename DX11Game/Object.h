/*=====ヘッダコメント=====
*@プログラム名	：Object.h
*@説明・概要	：回収するようアイテムについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/17
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "Main.h"
#include "Model.h"
#include "Land.h"
#include "Player.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

class CPlayer;
class CScore;
class CObject : public CModel
{
private:
	CPlayer* m_pPlayer;
	CCity* m_pCity;			// 街
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
