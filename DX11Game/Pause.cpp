/*=====�w�b�_�R�����g=====
*@�v���O������	�Fpause.cpp
*@�����E�T�v	�F�|�[�Y��ʂȂǂɂ���
*@�����	�FAT_12D_192_51�g�c����
*@�X�V��	�FAT_12D_192_51�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2021/12/20
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "Pause.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const int NUM_PAUSE_MENU = 3;			// �|�[�Y���j���[��
const float PAUSE_MENU_WIDTH = 320.0f;	// �|�[�Y���j���[��
const float PAUSE_MENU_HEIGHT = 60.0f;	// �|�[�Y���j���[����
const float PAUSE_MENU_INTERVAL = 100.0f;			// �|�[�Y���j���[�Ԋu
const float PAUSE_MENU_POS_X = 0.0f;				// �|�[�Y���j���[�ʒu(X���W)
const float PAUSE_MENU_POS_Y = PAUSE_MENU_INTERVAL;	// �|�[�Y���j���[�ʒu(Y���W)
const float PLATE_WIDTH = 360.0f;		// �v���[�g�̕�
const float PLATE_HEIGHT = 340.0f;		// �v���[�g�̕�
const float PLATE_POS_X = 0.0f;			// �v���[�g�̈ʒu(X���W)
const float PLATE_POS_Y = 0.0f;			// �v���[�g�̈ʒu(Y���W)
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;		//���X�e�B�b�N�̃f�b�h�]�[��
const int MAX_TEXTURE = 3;				//�ő�e�N�X�`��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[6] = {nullptr};	// �e�N�X�`���ւ̃|�C���^

static PAUSE_MENU g_nSelectPMenu = PAUSE_MENU_CONTINUE;		// �I�𒆂̃��j���[No.

static LPCWSTR c_aFileNamePauseMenu[MAX_TEXTURE] =
{
	L"data/texture/retry.png",	// ���g���C
	L"data/texture/select.png",	// �Z���N�g
	L"data/texture/rule.png",	// ���[��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			c_aFileNamePauseMenu[nCntPauseMenu],	// �t�@�C���̖��O
			&g_pTextures[nCntPauseMenu]);			// �ǂݍ��ރ������[
	}

	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	m_fCol = 0.0f;
	m_fCurve = 0.0f;
	m_bJoyStick = false;	//������������߂̃t���O

	// ���ʉ�������
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Fin()
{
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update()
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	CSound::SetVolume(SE_PI, 0.2f);
	//�f�b�h�]�[���ȉ��Ȃ炷�ׂĂO�ɂ���
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//���X�e�B�b�N����ɉ����ꂽ
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			m_bJoyStick = true;
		}
		//���X�e�B�b�N�����ɉ����ꂽ
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false)
		{
			CSound::Play(SE_PI);
			g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
			m_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			m_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_W)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}
	else if ( CInput::GetKeyRepeat(VK_S)) {
		CSound::Play(SE_PI);
		g_nSelectPMenu = (PAUSE_MENU)((g_nSelectPMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	m_fCurve += XM_PI * 0.01f;
	if (m_fCurve > XM_PI) {
		m_fCurve -= XM_2PI;
	}

	// ���ˌ��̐ݒ�
	m_fCol = cosf(m_fCurve) * 0.2f + 0.8f;

}


//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(PLATE_WIDTH, PLATE_HEIGHT);
	CPolygon::SetPos(PLATE_POS_X, PLATE_POS_Y);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetColor(m_fCol, m_fCol, m_fCol, 1.0f);
	CPolygon::Draw(pDeviceContext);

	CPolygon::SetSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; ++nCntPauseMenu) {
		CPolygon::SetPos(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);

		if (nCntPauseMenu == g_nSelectPMenu) {
			CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			//�I�΂�ĂȂ��ق�
			CPolygon::SetColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// �e�N�X�`���̐ݒ�
		CPolygon::SetTexture(g_pTextures[nCntPauseMenu]);
		// �|���S���̕`��
		CPolygon::Draw(pDeviceContext);
	}

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetZBuffer(true);
	SetBlendState(BS_NONE);
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void  CPause::SetPauseMenu()
{
	m_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU CPause::GetPauseMenu()
{
	return g_nSelectPMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void CPause::ResetPauseMenu()
{
	g_nSelectPMenu = PAUSE_MENU_CONTINUE;
	CPause::SetPauseMenu();
}
