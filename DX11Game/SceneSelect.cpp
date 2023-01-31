/*=====�w�b�_�R�����g=====
*@�v���O������	�FSceneSelect.cpp
*@�����E�T�v	�F�X�e�[�W�Z���N�g�V�[���ɂ���
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
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
const float POS_X_BG = 0.0f;			//�w�i��X���W
const float POS_Y_BG = 0.0f;			//�w�i��Y���W
const int WIDTH_BG = SCREEN_WIDTH;		//����
const int HEIGHT_BG = SCREEN_HEIGHT;	//����
const int MAX_TEXTURE = 1;				//�g���摜��
const int BLINK_TIMER = 30;			//�_�Ŏ��ԂŎg�p
const int BLINK_START_TIMER = 5;			//�_�ŊJ�n����

//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
namespace {
	const LPCWSTR g_pszPathTexBG = L"data/texture/rule.jpg";
}

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];


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
	m_nBlink = 0;
	m_nStart = 0;
	//��񂾂��Đ�������
	bSound1 = false;
	bSound2 = false;
	bSound3 = false;
	// BGM�Đ�
	CSound::SetVolume(BGM_TITLE, 0.1f, 0.0f);
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
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[��1��ʂ�
			m_nStart = 1; // �J�n�t���O
			m_nBlink = BLINK_START_TIMER;
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
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
			m_nStart = 1; // �J�n�t���O
			m_nBlink = BLINK_START_TIMER;
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
		if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//�G���^�[����
			CSound::SetVolume(SE_SELECT, 0.2f);
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
			m_nStart = 1; // �J�n�t���O
			m_nBlink = BLINK_START_TIMER;
			return;
		}		
		break;
	}
	//����������
	if (CInput::GetKeyPress(VK_X) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON4)) //Y��������
	{
		CFade::Out(SCENE_RULE);	//���[����ʂ�
		m_nStart = 1; // �J�n�t���O
		m_nBlink = BLINK_START_TIMER;
		return;
	}
	// �f�o�b�N�p�V�[���J��
	if (CInput::GetKeyRelease(VK_1)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_TITLE);	//�^�C�g����ʂ�
		m_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_RULE);	//���[����ʂ�
		m_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_SELECT);	//�Z���N�g��ʂ�
		m_nStart = 1; // �J�n�t���O
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
		m_nStart = 1; // �J�n�t���O
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
	CPolygon::SetPos(POS_X_BG, POS_Y_BG);
	CPolygon::Draw(pDC);

	//�X�e�[�W�Z���N�g�`��
	DrawSSelect();

}