/*=====�w�b�_�R�����g=====
*@�v���O������	�FTitleSelect.cpp
*@�����E�T�v	�F�^�C�g���Z���N�g����
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "titleSelect.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const int NUM_SELECT_MENU = 2;					// �Z���N�g���j���[��
const float SELECT_MENU_WIDTH = 500.0f;			// �Z���N�g���j���[��
const float	SELECT_MENU_HEIGHT = 100.0f;		// �Z���N�g���j���[����
const float SELECT_MENU_POS_X = 25.0f;			// �Z���N�g���j���[�ʒu(X���W)
const float SELECT_MENU_POS_Y = -150.0f;		// �Z���N�g���j���[�ʒu(Y���W)
const float SELECT_MENU_INTERVAL = 150.0f;		// �Z���N�g���j���[�Ԋu
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	//���X�e�B�b�N�̃f�b�h�]�[��
const int MAX_TEXTURE = 2;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[MAX_TEXTURE] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static TITLE_SELECT g_nSelectTMenu = GAME;		// �I�𒆂̃��j���[No.
static float m_fCurve = 0.0f;
static float m_fCol = 0.0f;
static int g_texture = 0;
static bool m_bJoyStick;

static LPCWSTR c_aFileNameTitleMenu[MAX_TEXTURE] =
{
	L"data/texture/start.png",	// game
	L"data/texture/end.png",	// end
};

//=============================================================================
// �X�e�[�W�Z���N�g����������
//=============================================================================
HRESULT CTSelect::Init(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNameTitleMenu[nCntPauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPauseMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectTMenu = GAME;
	m_fCurve = 0.0f;
	m_bJoyStick = false;

	// ���ʉ�������
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);
	//�ꎞ��~������
	return hr;
}

//=============================================================================
// �X�e�[�W�Z���N�g�I������
//=============================================================================
void CTSelect::Uninit(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// �X�e�[�W�Z���N�g�X�V����
//=============================================================================
void CTSelect::Update(void)
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	//�f�b�h�]�[���ȉ��Ȃ炷�ׂĂO�ɂ���
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//�W���C�X�e�B�b�N������
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false){
				CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			m_bJoyStick = true;
		}
		//�W���C�X�e�B�b�N����
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false){
				CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			m_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			m_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_UP))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}
	if (CInput::GetKeyRepeat(VK_DOWN))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}

	m_fCurve += XM_PI * 0.01f;
	if (m_fCurve > XM_PI) {
		m_fCurve -= XM_2PI;
	}

	// ���ˌ��̐ݒ�
	m_fCol = cosf(m_fCurve) * 0.2f + 0.8f;

}

//=============================================================================
// �X�e�[�W�Z���N�g�`�揈��
//=============================================================================
void CTSelect::Draw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntTitleMenu = 0; nCntTitleMenu < NUM_SELECT_MENU; ++nCntTitleMenu) {
		CPolygon::SetPos(SELECT_MENU_POS_X, SELECT_MENU_POS_Y - nCntTitleMenu * SELECT_MENU_INTERVAL);

		if (nCntTitleMenu == g_nSelectTMenu) {
			//�I��ł��郁�j���[�̕�
			CPolygon::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			CPolygon::SetSize(SELECT_MENU_WIDTH + 30.f, SELECT_MENU_HEIGHT + 30.f);
		}
		else {
			//�I�΂�ĂȂ��ق�
			CPolygon::SetColor(0.3f, 0.0f, 0.0f, 0.5f);
			CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
		}
		// �e�N�X�`���̐ݒ�
		CPolygon::SetTexture(g_pTextures[nCntTitleMenu]);

		// �|���S���̕`��
		CPolygon::Draw(pDeviceContext);
	}
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̐ݒ�
//=============================================================================
void CTSelect::SetTitleSelect(void)
{
	m_fCurve = 0.0f;
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̎擾
//=============================================================================
TITLE_SELECT CTSelect::GetTitleSelect(void)
{
	return g_nSelectTMenu;
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̃��Z�b�g
//=============================================================================
void CTSelect::ResetTitleSelect(void)
{
	g_nSelectTMenu = GAME;
	CSound::Play(SE_SHIZUKU);
	SetTitleSelect();
}
