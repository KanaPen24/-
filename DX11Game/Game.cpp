//=============================================================================
//
// �Q�[�� �N���X���� [Game.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Game.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "City.h"
#include "Score.h"

// �R���X�g���N�^
CGame::CGame() : CScene()
{
	m_id = SCENE_GAME;

	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
	for (int i = 0; i < _countof(m_pObject); ++i)
		m_pObject[i] = nullptr;

	m_nBoundary = 0;
}

// �f�X�g���N�^
CGame::~CGame()
{
}

// ������
bool CGame::Init()
{
	HRESULT hr = S_OK;

	m_bResult = false;

	m_camera.Init();
	CCamera::Set(&m_camera);

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}

	new CLand(this);

	new CCity(this);

	m_pPlayer = new CPlayer(this);

	m_pEnemy = new CEnemy(this);

	for (int i = 0; i < MAX_OBJECT; i++) {

		m_pObject[i] = new CObject(this);
		m_pObject[i]->SetPlayer(m_pPlayer);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixRotationRollPitchYaw(0.0f,XMConvertToRadians(rand() % 360),0.0f));
		mW._41 = - 400.0f + (float)(rand() % 400);
		mW._43 = (float)(rand() % 800);
		m_pObject[i]->SetWorld(mW);
	}

	//�X�R�A������
	hr = InitScore();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�X�R�A���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �S�I�u�W�F�N�g������
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	m_camera.SetPlayer(m_pPlayer);

	// BGM�Đ��J�n
	CSound::Play(BGM_GAME);

	return true;
}

// �I������
void CGame::Fin()
{
	// BGM�Đ���~
	CSound::Stop(BGM_GAME);

	//�X�R�A�I������
	UninitScore();

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);
}

// �X�V
void CGame::Update()
{
	// �S�L�����X�V
	CGameObj::UpdateAll(m_pObj);

	if (!m_bResult) {
		if (CInput::GetKeyRelease(VK_BACK) ||
			CInput::GetKeyRelease(VK_DELETE)) {
			m_bResult = true;
			CFade::Out(SCENE_TITLE);
		}
	}

	// �X�R�A�̍X�V
	UpdateScore();

#ifdef _DEBUG
	//static LPCSTR boundary[] = {"�ˮ���", "���"};
	//CDebugProc::Print("[B] : ����� %s\n", boundary[m_nBoundary]);
	//CDebugProc::Print("\n");
#endif // !_DEBUG

	
}

// �`��
void CGame::Draw()
{
	// �S�L�����`��
	CGameObj::DrawAll(m_pObj);

	// �X�R�A�`��
	DrawScore();
}

// ���E�\���X�V
void CGame::UpdateBoundary()
{
	for (CGameObj* pObj = GetObj(); pObj; pObj = pObj->GetNext()) {
		switch (pObj->GetID()) {
		case GOT_PLAYER:
			switch (m_nBoundary) {
			case 0:	// ���E�\������
				pObj->SetBSphereVisible(false);
				break;
			case 1:	// ���E��
				pObj->SetBSphereVisible();
				break;
			}
			break;
		}
	}
}
