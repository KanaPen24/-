//=============================================================================
//
// �^�C�g����ʃN���X���� [Title.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "SceneTitle.h"
#include "Texture.h"
#include "Sound.h"
#include "Input.h"
#include "Polygon.h"
#include "Fade.h"
#include "TitleSelect.h"
#include "Land.h"
#include "Particle.h"

// �萔��`
namespace {
	const LPCWSTR g_pszPathTexLogo = L"data/texture/title2.png";

	const LPCWSTR g_pszPathTexBG = L"data/texture/sky001.jpg";

}

static bool bSound1;	//��񂾂�����
static bool bSound2;	//��񂾂�����
static bool bSound3;	//��񂾂�����

// �R���X�g���N�^
CTitle::CTitle() : CScene()
{
	m_id = SCENE_TITLE;

	m_bStart = false;
	m_pTexTitle = nullptr;
	m_pTexBG = nullptr;	
}

// �f�X�g���N�^
CTitle::~CTitle()
{
}

// ������
bool CTitle::Init()
{
	if (!CScene::Init()) {
		return false;
	}
	m_camera.Init();
	CCamera::Set(&m_camera);
	
	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	m_bStart = false;
	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, g_pszPathTexLogo, &m_pTexTitle);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	hr = CreateTextureFromFile(pDevice, g_pszPathTexBG, &m_pTexBG);
	if (FAILED(hr)) {
		Fin();
		return false;
	}
	//�^�C�g���Z���N�g������
	hr = InitTSelect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�^�C�g���Z���N�g���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	new CLand(this);
	m_pCity = new CCity(this);
	// �S�I�u�W�F�N�g������
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}
	//�J�������X�Ɍ�����
	m_camera.SetCity(m_pCity);

	// BGM�Đ��J�n
	CSound::Play(BGM_TITLE);
	CSound::SetVolume(BGM_TITLE, 0.2f, 0.0f);
	return true;
}

// �I������
void CTitle::Fin()
{
	// BGM�Đ���~
	CSound::Stop(BGM_TITLE);

	SAFE_RELEASE(m_pTexBG);
	SAFE_RELEASE(m_pTexTitle);

	//�^�C�g���Z���N�g�I��
	UninitTSelect();

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);

	CScene::Fin();
}

// �X�V
void CTitle::Update()
{
	//�^�C�g���Z���N�g�X�V
	UpdateTSelect();

	//�p�[�e�B�N���X�V
	UpdateParticle();

	CSound::SetVolume(SE_SELECT, 0.1f);
	switch (GetTitleSelect())
	{
	case GAME:
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
		if (CInput::GetKeyRelease(VK_RETURN)) {//){ || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::Play(SE_SELECT);	//���ʉ�
			CFade::Out(SCENE_SELECT);	//�Q�[��1��ʂ�
			
		}
		break;
	case END:

		bSound1 = true;
		bSound3 = true;
		if (bSound2 == true)
		{
			CSound::SetVolume(SE_PI, 0.2f);
			CSound::Play(SE_PI);
			bSound2 = false;
		}
		if (CInput::GetKeyRelease(VK_RETURN)) {//){ || GetJoyTrigger(JOYSTICKID1, JOYBUTTON1)) {//�G���^�[����
			CSound::Play(SE_SELECT);	//���ʉ�
			PostQuitMessage(0);				// �V�X�e���ɃX���b�h�̏I����v��
		}
		break;
	}
	// �S�L�����X�V
	CGameObj::UpdateAll(m_pObj);
}

// �`��
void CTitle::Draw()
{
	HDC hdc;
	PAINTSTRUCT	ps;
	HWND hWnd = GetMainWnd();
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	//�w�i�`��
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetTexture(m_pTexBG);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);
	SetZBuffer(true);
	SetBlendState(BS_NONE);

	// ���f���`��
	CGameObj::DrawAll(m_pObj);
	
	//�p�[�e�B�N���`��
	DrawParticle();

	//�^�C�g�����S�`��
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetTexture(m_pTexTitle);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::Draw(pDC);

	//�^�C�g���Z���N�g�`��
	DrawTSelect();
	
	// �p�[�e�B�N���`��
	/*hdc = BeginPaint(hWnd, &ps);
	CParticle::ExecParticle();
	CParticle::DrawParticle(hdc);
	EndPaint(hWnd, &ps);*/
	SetZBuffer(true);

}
