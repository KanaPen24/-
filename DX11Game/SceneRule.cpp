/*=====�w�b�_�R�����g=====
*@�v���O������	�FSceneRule.cpp
*@�����E�T�v	�F���[���V�[���ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneRule.h"
#include "Polygon.h"
#include "Texture.h"
#include "Input.h"
#include "Fade.h"
#include "Sound.h"
#include "Scene.h"
#include "Debugproc.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const int COUNT_APPEAR = 60;		// �X�^�[�g�{�^���o���܂ł̎���
const int INTERVAL_DISP = 60;		// �X�^�[�g�{�^���_�ł̎���


const int BLINK_TIMER = 30;			//�_�Ŏ��ԂŎg�p
const int BLINK_START_TIMER = 5;			//�_�ŊJ�n����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexBG = L"data/texture/setumei.jpg";
}
static ID3D11ShaderResourceView*	g_pTextures[1];			// �e�N�X�`���ւ̃|�C���^
static int							g_nCountAppear = 0;		//
static int							g_nCountDisp = 0;		//
static bool							g_bDispStart = false;	//
static bool							g_bSkip = false;		// �X�^�[�g�{�^������
static int g_nBlink;
static int g_nStart;

CRule::CRule() : CScene()
{
	m_id = SCENE_RULE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CRule::~CRule()
{

}

//=============================================================================
// ���[����ʂ̏���������
//=============================================================================
bool CRule::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}

	// �ϐ�������
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bSkip = false;

	// BGM�Đ��J�n
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// ���[����ʂ̏I������
//=============================================================================
void CRule::Fin(void)
{
	// BGM�Đ���~
	//CSound::Stop(BGM_TITLE);

	// �e�N�X�`�����
	SAFE_RELEASE(m_pTexBG);
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ���[����ʂ̍X�V����
//=============================================================================
void CRule::Update(void)
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0) {
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bSkip) {
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]�܂��̓X�y�[�X�L�[�ŊJ�n
	//if (!g_bSkip && (CInput::GetKeyRelease(VK_RETURN) || CInput::GetKeyRelease(VK_SPACE)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1))) {
	//	// �I����
	//	//CSound::Play(SE_SELECT);
	//	// �t�F�[�h�A�E�g��V�[���J��
	//	CFade::Out(SCENE_TITLE);	//�^�C�g���Ɉړ�

	//	g_bSkip = true;
	//}
	// �^�C�g����ʂ�
	if (CInput::GetKeyRelease(VK_RETURN)){ //GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_SELECT);//�Z���N�g�Ɉړ�
		g_nStart = 1; // �J�n�t���O
		return;
	}
	// �f�o�b�N�p�V�[���J��
	if (CInput::GetKeyRelease(VK_1)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_TITLE);	//�^�C�g����ʂ�
		g_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_RULE);	//���[����ʂ�
		g_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_SELECT);	//�Z���N�g��ʂ�
		g_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
		g_nStart = 1; // �J�n�t���O
		return;
	}

#ifdef _DEBUG
	CDebugProc::Print("����:1\n");
	CDebugProc::Print("ٰ�  :2\n");
	CDebugProc::Print("�ڸ� :3\n");
	CDebugProc::Print("�ް� :4\n");
#endif // _DEBUG
}

//=============================================================================
// �`���[�g���A����ʕ`��
//=============================================================================
void CRule:: Draw(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

}
