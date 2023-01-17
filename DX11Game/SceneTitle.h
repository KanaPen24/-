/*=====ヘッダコメント=====
*@プログラム名	：SceneTitle.h
*@説明・概要	：タイトルシーンについて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
#include "Scene.h"
#include "TitleCamera.h"
#include "City.h"

// クラス定義
class CTitle : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	CTitleCamera m_camera;
	CCity* m_pCity;
	int m_nTimer;

public:
	CTitle();
	virtual ~CTitle();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
