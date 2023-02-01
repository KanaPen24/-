/*=====�w�b�_�R�����g=====
*@�v���O������	�FGame.cpp
*@�����E�T�v	�F�Q�[���V�[���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Game.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "Debugproc.h"
#include "City.h"
#include "Score.h"
#include "Radar.h"
#include "StegeSelect.h"

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

	CCamera::Set(&m_camera);

	new CLand(this);

	new CCity(this);

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	switch (GetStageSelect())
	{
	case STAGE_1:
		m_nCntObj = 1;
		break;
	case STAGE_2:
		m_nCntObj = 10;
		break;
	case STAGE_3:
		m_nCntObj = 20;
		break;
	}	

	m_pPlayer = new CPlayer(this);

	m_pEnemy = new CEnemy(this);
	m_pEnemy->SetPlayer(m_pPlayer);
	

	for (int i = 0; i < m_nCntObj; i++) {

		m_pObject[i] = new CObject(this);
		m_pObject[i]->SetPlayer(m_pPlayer);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, XMMatrixRotationRollPitchYaw(0.0f,XMConvertToRadians(rand() % 360),0.0f));
		mW._41 = - 500.0f + (float)(rand() % 500);
		mW._43 = (float)(200+rand() % 800);
		m_pObject[i]->SetWorld(mW);
	}

	//�X�R�A������
	m_pScore = new CScore;
	hr =m_pScore->Init(this);
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�X�R�A���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ���[�_�[ �e�N�X�`���Ǎ�
	if (FAILED(CRadar::LoadTexture())) {
		return false;
	}

	// �S�I�u�W�F�N�g������
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}
	
	//�J�����v���C���[�ɐݒ�
	m_camera.SetPlayer(m_pPlayer);
	m_camera.Init();

	// ���[�_�[�����A������
	m_pRadar = new CRadar();
	m_pRadar->Init(this);

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
	m_pScore->Uninit();

	// ���[�_�[ �e�N�X�`�����
	CRadar::ReleaseTexture();

	m_camera.Fin();

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

	// ���[�_�[�X�V
	m_pRadar->Update();

	// �X�R�A�̍X�V
	m_pScore->Update();

#ifdef _DEBUG
	//static LPCSTR boundary[] = {"�ˮ���", "���"};
	//CDebugProc::Print("[B] : ����� %s\n", boundary[m_nBoundary]);
	//CDebugProc::Print("\n");
#endif // !_DEBUG

	
}

// �`��
void CGame::Draw()
{
	SetBlendState(BS_ALPHABLEND);
	// �S�L�����`��
	CGameObj::DrawAll(m_pObj);

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	// �X�R�A�`��
	m_pScore->Draw();
	// ���[�_�[�`��
	m_pRadar->Draw();
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
