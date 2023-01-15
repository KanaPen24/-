/*=====�w�b�_�R�����g=====
*@�v���O������	�Fgameover.cpp
*@�����E�T�v	�F�Q�[���I�[�o�[��ʂɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include "SceneGameover.h"
#include "Texture.h"
#include "Polygon.h"
#include "Input.h"
#include "Scene.h"
#include "Sound.h"
#include "Fade.h"
#include "Time.h"

// �}�N����`
#define PATH_BGTEXTURE L"data/texture/�Q�[���I�[�o�[�f�b�h�G���h��.png"

const float BG_POS_X = 0.0f;
const float BG_POS_Y = 0.0f;
const int BG_WIDTH = SCREEN_WIDTH;
const int BG_HEIGHT = SCREEN_HEIGHT;
const int GAMEOVER_TIMER = 250;
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
// �R���X�g���N�^
CGOver::CGOver() : CScene()
{
	m_id = SCENE_GAMEOVER;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;

}

// �f�X�g���N�^
CGOver::~CGOver()
{
}

// ������
bool CGOver::Init() {
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_BGTEXTURE, &g_pTexture);
	if(FAILED(hr)) {
		return hr;
	}
	// BGM�Đ�
	//CSound::Play(SE_GAMEOVER);
	return hr;
}
// �I������
void CGOver::Fin() {
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);

}
// �X�V
void CGOver::Update() {
	// [Enter]�܂��̓X�y�[�X�L�[����
		// [Enter]�܂��̓X�y�[�X�L�[����
	if (CInput::GetKeyRelease(VK_RETURN)){// || GetKeyRelease(VK_SPACE)||GetJoyButton(JOYSTICKID1,JOYBUTTON1)) {
		// �^�C�g����ʂ�
		CFade::Out(SCENE_SELECT);
		return;
	}
}
// �`��
void CGOver::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CPolygon::SetSize(BG_WIDTH, BG_HEIGHT);
	CPolygon::SetPos(BG_POS_X, BG_POS_Y);
	CPolygon::SetTexture(g_pTexture);
	CPolygon::Draw(pDC);
}