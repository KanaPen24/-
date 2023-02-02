/*=====�w�b�_�R�����g=====
*@�v���O������	�Fscore.cpp
*@�����E�T�v	�F�X�R�A�\���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*///*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Score.h"
#include "Number.h"
#include "Texture.h"
#include "Polygon.h"
#include "Debugproc.h"
#include "Fade.h"
#include "Scene.h"
#include "SceneClear.h"

// �}�N����`
const int CHRCNT_SCORE = 2;
const int POS_X_SCORE = 0.0f;
const float POS_Y_SCORE = ((SCREEN_HEIGHT - 80.0f) * 0.5f + 16.0f);
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
int g_CntObj;

// ������
HRESULT CScore::Init(CGame* pScene)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �ϐ�������
	m_pScene = pScene;
	m_pObj = nullptr;
	m_nScore = m_pScene->m_nCntObj;
	g_CntObj = m_nScore;
	return hr;
}
// �I������
void CScore::Fin()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
// �X�V
void CScore::Update()
{
	if (g_CntObj == 0)
	{
		CFade::CutOut(SCENE_CLEAR);
		CClear::Clearflg(true);
	}
}
// �`��
void CScore::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// ���邢��
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	// �X�R�A�\��
	DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)g_CntObj, CHRCNT_SCORE);
	// ���ɖ߂�
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}
void CScore::Add(int nScore) 
{  
	g_CntObj+= nScore;
}