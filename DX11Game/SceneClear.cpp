/*=====�w�b�_�R�����g=====
*@�v���O������	�FClear.cpp
*@�����E�T�v	�F�N���A��ʂȂǂɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/12/20
*/

//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "SceneClear.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"
#include "StegeSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_CLEAR_MENU		(3)			// �N���A���j���[��
#define	CLEAR_MENU_WIDTH	(320.0f)	// �N���A���j���[��
#define	CLEAR_MENU_HEIGHT	(60.0f)		// �N���A���j���[����
#define	CLEAR_MENU_POS_X	(-380.0f)	// �N���A���j���[�ʒu(X���W)
#define	CLEAR_MENU_POS_Y	(-250.0f)	// �N���A���j���[�ʒu(Y���W)
#define	CLEAR_MENU_INTERVAL	(400.0f)	// �N���A���j���[�Ԋu
#define	PLATE_WIDTH			(SCREEN_WIDTH)	// �v���[�g�̕�
#define	PLATE_HEIGHT		(50.0f)		// �v���[�g�̍���
#define	PLATE_POS_X			(0.0f)		// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y			(CLEAR_MENU_POS_Y)		// �v���[�g�̈ʒu(Y���W)
#define GAMEPAD_LEFT_STICK_DEADZONE  (7800)	//���X�e�B�b�N�̃f�b�h�]�[��
#define MAX_TEXTURE			(4)
#define POS_X_BG			(0.0f)			//�w�i��X���W
#define POS_Y_BG			(0.0f)			//�w�i��Y���W
#define WIDTH_BG			(SCREEN_WIDTH)	//����
#define HEIGHT_BG			(SCREEN_HEIGHT)	//����
#define TEX_BG				(0)				//�w�i�̉摜��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[4] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static CLEAR_MENU g_nSelectCMenu = CLEAR_MENU_SELECT;		// �I�𒆂̃��j���[No.
static float	g_fCurve = 0.0f;
//static float	g_fCol = 0.0f;
static bool		g_bJoyStick;
static bool		g_bSound1;		// ��񂾂�����
static bool		g_bSound2;		// ��񂾂�����
static bool		g_bSound3;		// ��񂾂�����
static bool		g_bClear;		//�N���A�t���O

static LPCWSTR c_aFileNameClearMenu[MAX_TEXTURE] =
{
	L"data/texture/stageclear.jpg",	// �w�i
	L"data/texture/select.png",	// �Z���N�g
	L"data/texture/retry.png",	// ���g���C
	L"data/texture/Gtitle.png"
};
// �R���X�g���N�^
CClear::CClear() : CScene()
{
	m_id = SCENE_CLEAR;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;

}

// �f�X�g���N�^
CClear::~CClear()
{
}

//=============================================================================
// ����������
//=============================================================================
bool CClear::Init(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntClearMenu = 0; nCntClearMenu < MAX_TEXTURE; ++nCntClearMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNameClearMenu[nCntClearMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntClearMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectCMenu = CLEAR_MENU_SELECT;
	g_fCurve = 0.0f;
	g_bJoyStick = false;
	g_bClear = false;

	// ���ʉ�������
	// BGM�Đ�
	CSound::SetVolume(BGM_TITLE, 0.04f);
	CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CClear::Fin(void)
{
	// ����~
	CSound::Stop(BGM_TITLE);
	CSound::Stop(SE_SELECT);

	// �e�N�X�`���̊J��
	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		SAFE_RELEASE(g_pTextures[nCntClearMenu]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CClear::Update(void)
{
	//DWORD JoyCount = GetJoyCount();
	//LONG JoyX = GetJoyX(JOYSTICKID1);
	//CSound::SetVolume(SE_PI, 0.2f);
	////�f�b�h�]�[���ȉ��Ȃ炷�ׂĂO�ɂ���
	//if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
	//	JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	//{
	//	JoyX = 0l;
	//}

	//if (JoyCount >= 1) {
	//	//���X�e�B�b�N�����ɉ����ꂽ
	//	if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
	//	{
	//		CSound::Play(SE_PI);
	//		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
	//		SetClearMenu();
	//		g_bJoyStick = true;
	//	}
	//	//���X�e�B�b�N���E�ɉ����ꂽ
	//	if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false)
	//	{
	//		CSound::Play(SE_PI);
	//		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
	//		SetClearMenu();
	//		g_bJoyStick = true;
	//	}
	//	if (JoyX == 0l)
	//	{
	//		g_bJoyStick = false;
	//	}
	//}
	if (CInput::GetKeyRepeat(VK_LEFT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + NUM_CLEAR_MENU - 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}
	else if (CInput::GetKeyRepeat(VK_RIGHT)) {
		CSound::Play(SE_PI);
		g_nSelectCMenu = (CLEAR_MENU)((g_nSelectCMenu + 1) % NUM_CLEAR_MENU);
		SetClearMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}
	//�Q�[���N���A���j���[�̑I��
	if (g_bClear&&CFade::Get() == FADE_NONE) {
		//[Enter]�������ꂽ�H
		if (CInput::GetKeyTrigger(VK_RETURN)){// || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {
			//�I�𒆂̃��j���[���ڂɂ�蕪��
			switch (GetClearMenu())
			{
			case CLEAR_MENU_SELECT:
				CFade::Out(SCENE_SELECT);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_RETRY:
				CFade::Out(SCENE_GAME);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			case CLEAR_MENU_TITLE:
				CFade::Out(SCENE_TITLE);
				CSound::SetVolume(SE_SELECT, 0.2f);
				CSound::Play(SE_SELECT);
				break;
			}
		}
		switch (GetClearMenu())
		{
		case CLEAR_MENU_SELECT:
			g_bSound2 = true;
			g_bSound3 = true;

			if (g_bSound1 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound1 = false;
			}
			break;

		case CLEAR_MENU_RETRY:
			g_bSound1 = true;
			g_bSound3 = true;

			if (g_bSound2 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound2 = false;
			}
			break;

		case CLEAR_MENU_TITLE:
			g_bSound1 = true;
			g_bSound2 = true;

			if (g_bSound3 == true)
			{
				CSound::SetVolume(SE_PI, 0.2f);
				CSound::Play(SE_PI);
				g_bSound3 = false;
			}
			break;
		}
	}
	//// ���ˌ��̐ݒ�
	//g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// �`�揈��
//=============================================================================
void CClear::Draw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(WIDTH_BG, HEIGHT_BG);
	CPolygon::SetPos(POS_X_BG, POS_Y_BG);
	CPolygon::SetTexture(g_pTextures[0]);
	CPolygon::Draw(pDeviceContext);

	
	
	for (int nCntClearMenu = 0; nCntClearMenu < NUM_CLEAR_MENU; ++nCntClearMenu) {
		CPolygon::SetPos(CLEAR_MENU_POS_X + nCntClearMenu * CLEAR_MENU_INTERVAL, CLEAR_MENU_POS_Y);

		if (nCntClearMenu == g_nSelectCMenu) {
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			CPolygon::SetSize(CLEAR_MENU_WIDTH + 30.0f, CLEAR_MENU_HEIGHT + 80.0f);
		}
		else {
			//�I�΂�ĂȂ��ق�
			CPolygon::SetColor(0.3f, 0.3f, 0.3f, 0.5);
			CPolygon::SetSize(CLEAR_MENU_WIDTH, CLEAR_MENU_HEIGHT);
		}

		// �e�N�X�`���̐ݒ�
		CPolygon::SetTexture(g_pTextures[nCntClearMenu+1]);
		// �|���S���̕`��
		CPolygon::Draw(pDeviceContext);
	}

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �N���A���j���[�̐ݒ�
//=============================================================================
void CClear::SetClearMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �N���A���j���[�̎擾
//=============================================================================
CLEAR_MENU CClear::GetClearMenu(void)
{
	return g_nSelectCMenu;
}

//=============================================================================
// �N���A���j���[�̃��Z�b�g
//=============================================================================
void CClear::ResetClearMenu(void)
{
	g_nSelectCMenu = CLEAR_MENU_SELECT;
	SetClearMenu();
}
//=============================================================================
// �N���A��ʕ\���t���O
// @param	[bool]	Clearflg	�N���A�������ǂ���
// @return	[bool]	g_bClear	�N���A���
//=============================================================================
bool CClear::Clearflg(bool Clearflg)
{
	g_bClear = Clearflg;
	return g_bClear;
}