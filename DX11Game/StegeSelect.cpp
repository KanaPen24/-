/*=====�w�b�_�R�����g=====
*@�v���O������	�FStageSelect.cpp
*@�����E�T�v	�F�X�e�[�W�Z���N�g����
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/11/28
*/
#include "StegeSelect.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const int NUM_SELECT_MENU = 3;					// �Z���N�g���j���[��
const float SELECT_MENU_WIDTH = 360.0f;			// �Z���N�g���j���[��
const float SELECT_MENU_HEIGHT = 360.0f;		// �Z���N�g���j���[����
const float SELECT_MENU_POS_X = -420.0f;		// �Z���N�g���j���[�ʒu(X���W)
const float SELECT_MENU_POS_Y = 0.0f;			// �Z���N�g���j���[�ʒu(Y���W)
const float SELECT_MENU_INTERVAL = 400.0f;		// �Z���N�g���j���[�Ԋu
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	//���X�e�B�b�N�̃f�b�h�]�[��
const float UNDER_MOVE_RIMIT_Y = -10.f;			//���S�̓������̏���l
const float TOP_MOVE_RIMIT_Y = 10.f;			//���S�̓�����̏���l
const float ROGO_SPEED = 0.5f;					//���S�������X�s�[�h


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static STAGE_SELECT g_nSelectSMenu = STAGE_1;		// �I�𒆂̃��j���[No.
static float g_fCurve = 0.0f;
static float g_fCol = 0.0f;
static bool g_bJoyStick;
static bool flg;
static float g_fPosY;

static LPCWSTR c_aFileNameStageMenu[NUM_SELECT_MENU] =
{
	L"data/texture/stage1.png",	// �X�e�[�W1
	L"data/texture/stage2.png",	// �X�e�[�W2
	L"data/texture/stage3.png",	// �X�e�[�W3
};

//=============================================================================
// �X�e�[�W�Z���N�g����������
//=============================================================================
HRESULT InitSSelect(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			c_aFileNameStageMenu[nCntSelectMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntSelectMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectSMenu = STAGE_1;
	g_fCurve = 0.0f;
	g_bJoyStick = false;
	flg = true;
	g_fPosY = SELECT_MENU_POS_Y;


	// ���ʉ�������
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// �X�e�[�W�Z���N�g�I������
//=============================================================================
void UninitSSelect(void)
{
	// �e�N�X�`���̊J��
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		SAFE_RELEASE(g_pTextures[nCntSelectMenu]);
	}
}

//=============================================================================
// �X�e�[�W�Z���N�g�X�V����
//=============================================================================
void UpdateSSelect(void)
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0l;
	}
	if (JoyCount >= 1) {
		if (JoyX <= -GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
				CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX >= GAMEPAD_LEFT_STICK_DEADZONE && g_bJoyStick == false) {
				CSound::Play(SE_SHIZUKU);
			g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
			SetStageSelect();
			g_bJoyStick = true;
		}
		if (JoyX == 0l)
		{
			g_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_LEFT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}
	if (CInput::GetKeyRepeat(VK_RIGHT))
	{
		g_nSelectSMenu = (STAGE_SELECT)((g_nSelectSMenu + 1) % NUM_SELECT_MENU);
		SetStageSelect();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) {
		g_fCurve -= XM_2PI;
	}

	// ���ˌ��̐ݒ�
	g_fCol = cosf(g_fCurve) * 0.2f + 0.8f;

	//���S�̂ӂ�ӂ튴
	if (flg) {
		g_fPosY = g_fPosY - ROGO_SPEED;
	}
	if (!flg) {
		g_fPosY = g_fPosY + ROGO_SPEED;
	}
	if (g_fPosY <= UNDER_MOVE_RIMIT_Y) flg = false;
	if (g_fPosY >= TOP_MOVE_RIMIT_Y)flg = true;


}

//=============================================================================
// �X�e�[�W�Z���N�g�`�揈��
//=============================================================================
void DrawSSelect(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntSelectMenu = 0; nCntSelectMenu < NUM_SELECT_MENU; ++nCntSelectMenu) {
		CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);

		if (nCntSelectMenu == g_nSelectSMenu) {
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, g_fPosY);
		}
		else {
			CPolygon::SetColor(0.0f, 0.0f, 0.0f, 0.5f);
			CPolygon::SetPos(SELECT_MENU_POS_X + nCntSelectMenu * SELECT_MENU_INTERVAL, SELECT_MENU_POS_Y);
		}
		// �e�N�X�`���̐ݒ�
		CPolygon::SetTexture(g_pTextures[nCntSelectMenu]);
		// �|���S���̕`��
		CPolygon::Draw(pDeviceContext);
	}
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̐ݒ�
//=============================================================================
void SetStageSelect(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̎擾
//=============================================================================
STAGE_SELECT GetStageSelect(void)
{
	return g_nSelectSMenu;
}

//=============================================================================
// �X�e�[�W�Z���N�g���j���[�̃��Z�b�g
//=============================================================================
void ResetStageSelect(void)
{
	g_nSelectSMenu = STAGE_1;
	//CSound::Play(SE_SHIZUKU);
	SetStageSelect();
}
