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
#include "scenePrologue.h"
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
	const LPCWSTR g_pszPathTexBG = L"data/texture/Prologue.jpg";
}
static ID3D11ShaderResourceView*	g_pTextures[1];			// �e�N�X�`���ւ̃|�C���^

CPrologue::CPrologue() : CScene()
{
	m_id = SCENE_PROLOGUE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexStart = nullptr;
	m_pTexBG = nullptr;
}

CPrologue::~CPrologue()
{

}

//=============================================================================
// ���[����ʂ̏���������
//=============================================================================
bool CPrologue::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}


	return hr;
}

//=============================================================================
// ���[����ʂ̏I������
//=============================================================================
void CPrologue::Fin(void)
{

	// �e�N�X�`�����
	SAFE_RELEASE(m_pTexBG);
	CSound::Stop(SE_SELECT);
}

//=============================================================================
// ���[����ʂ̍X�V����
//=============================================================================
void CPrologue::Update(void)
{
	// �Z���N�g��ʂ�
	if (CInput::GetKeyRelease(VK_RETURN) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {//�G���^�[����
		CSound::SetVolume(SE_SELECT, 0.3f);
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_TITLE);//�Z���N�g�Ɉړ�
		return;
	}
	// �f�o�b�N�p�V�[���J��
	if (CInput::GetKeyRelease(VK_1)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_TITLE);	//�^�C�g����ʂ�
		return;
	}
	if (CInput::GetKeyRelease(VK_2)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_RULE);	//���[����ʂ�
		return;
	}
	if (CInput::GetKeyRelease(VK_3)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_SELECT);	//�Z���N�g��ʂ�
		return;
	}
	if (CInput::GetKeyRelease(VK_4)) {//�G���^�[����
		CSound::Play(SE_SELECT);	//���ʉ�
		CFade::Out(SCENE_GAME);	//�Q�[����ʂ�
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
void CPrologue:: Draw(void)
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
