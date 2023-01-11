/*=====�w�b�_�R�����g=====
*@�v���O������	�FsceneSelect.cpp
*@�����E�T�v	�F�^�C�g���V�[���ɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "sceneSelect.h"
#include "texture.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Debugproc.h"
#include "StegeSelect.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POS_X_BG		(0.0f)			//�w�i��X���W
#define POS_Y_BG		(0.0f)			//�w�i��Y���W
#define WIDTH_BG		(SCREEN_WIDTH)	//����
#define HEIGHT_BG		(SCREEN_HEIGHT)	//����
#define TEX_BG			(0)				//�w�i�̉摜��
#define TEX_ENTER		(1)				//�G���^�[�摜��
#define MAX_TEXTURE		(1)				//�g���摜��
#define BLINK_TIMER		(30)			//�_�Ŏ��ԂŎg�p
#define BLINK_START_TIMER	(5)			//�_�ŊJ�n����

//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexBG = L"data/texture/rule.jpg";
}

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nBlink;
static int g_nStart;

static bool bSound1;	//��񂾂�����
static bool bSound2;	//��񂾂�����
static bool bSound3;	//��񂾂�����

CSelect::CSelect() : CScene()
{
	m_id = SCENE_SELECT;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CSelect::~CSelect()
{

}

//=============================================================================
// �Z���N�g�V�[���̏���������
//=============================================================================
bool CSelect::Init() 
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	// �ϐ�������
	g_nBlink = 0;
	g_nStart = 0;
	//��񂾂��Đ�������
	bSound1 = false;
	bSound2 = false;
	bSound3 = false;
	// BGM�Đ�
	CSound::SetVolume(BGM_TITLE, 0.1f);
	CSound::Play(BGM_TITLE);
	//�ꎞ��~������
	hr = InitSSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�X�e�[�W�Z���N�g���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	return true;
}
//=============================================================================
// �Z���N�g�V�[���̏I������
//=============================================================================
void CSelect::Fin()
{
	// ����~
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);
	SAFE_RELEASE(m_pTexBG);
	//�X�e�[�W�Z���N�g�I��
	UninitSSelect();
}
//=============================================================================
// �Z���N�g�V�[���̍X�V����
//=============================================================================
void CSelect::Update() {
	//�X�e�[�W�Z���N�g�X�V
	UpdateSSelect();
	
	switch (GetStageSelect())
	{
	case STAGE_1:
		bSound2 = true;
		bSound3 = true;
		//CSound::Resume();
		if (bSound1 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound1 = false;
		}
		// �J�n
		if (CInput::GetKeyRelease(VK_RETURN)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[��1��ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}
		break;
	case STAGE_2:
		
		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	case STAGE_3:
		bSound1 = true;
		bSound2 = true;
		if (bSound3 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound3 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
			g_nStart = 1; // �J�n�t���O
			g_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	}
	//����������
	if (CInput::GetKeyPress(VK_X))//||GetJoyTrigger(JOYSTICKID1, JOYBUTTON4)) //Y��������
	{
		CFade::Out(SCENE_RULE);	//���[����ʂ�
		g_nStart = 1; // �J�n�t���O
		g_nBlink = BLINK_START_TIMER;
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
// �Z���N�g�V�[���̕`�揈��
//=============================================================================
void  CSelect::Draw() {

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

	//�X�e�[�W�Z���N�g�`��
	DrawSSelect();

}