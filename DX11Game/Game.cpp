/*=====�w�b�_�R�����g=====
*@�v���O������	�FGame.cpp
*@�����E�T�v	�F�Q�[���V�[���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/17
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Game.h"
#include "Score.h"
#include "Radar.h"
#include "Pause.h"
#include "PostProcess.h"


// �R���X�g���N�^
CGame::CGame() : CScene()
{
	m_id = SCENE_GAME;

	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
	m_pPause = nullptr;
	m_pRadar = nullptr;
	m_pScore = nullptr;
	m_pSSelect = nullptr;

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

	//�ϐ�������
	m_bResult = false;
	m_bPause = false;
	m_fAlf = 0.0f;

	CCamera::Set(&m_camera);

	new CLand(this);

	new CCity(this);

	m_pSSelect = new CSSelect;	

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}
	switch (m_pSSelect->GetStageSelect())
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
		mW._41 = - 900.0f + (float)(rand() % 1000);
		mW._42 = 17.0f;
		mW._43 = (float)(200+rand() % 1000);
		m_pObject[i]->SetWorld(mW);
	}

	//�X�R�A������
	m_pScore = new CScore;
	hr =m_pScore->Init(this);
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�X�R�A���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�ꎞ��~������
	m_pPause = new CPause;
	hr = m_pPause->Init();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�|�[�Y���������s"), NULL, MB_OK | MB_ICONSTOP);
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

	//�|�X�g�v���Z�X������
	CPostProcess::Init();

	// BGM�Đ��J�n&���ʐݒ�
	CSound::Play(BGM_GAME);
	CSound::SetVolume(SE_SHIZUKU, 0.2f);
	CSound::SetVolume(SE_CANCEL, 0.2f);
	return true;
}

// �I������
void CGame::Fin()
{
	// BGM�Đ���~
	CSound::Stop(BGM_GAME);

	//�X�R�A�I������
	m_pScore->Fin();

	//�|�[�Y�I��
	m_pPause->Fin();

	// ���[�_�[ �e�N�X�`�����
	CRadar::ReleaseTexture();

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);
}

// �X�V
void CGame::Update()
{
	//�ꎞ��~���H
	if (m_bPause) {
		//�ꎞ��~�X�V
		m_pPause->Update();
	}
	else
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
	}
	//�ꎞ��~ON/OFF
	if (CInput::GetKeyTrigger(VK_Q) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON8)) {
		//�t�F�[�h�C��/�A�E�g���łȂ����
		if (CFade::Get() == FADE_NONE) {
			m_bPause = !m_bPause;
			if (m_bPause) {
				//�ꎞ��~�J�n
				CSound::Pause();
				CSound::Play(SE_SHIZUKU);
				if (m_bSoundPause == true)	//���������񂾂��Đ�
				{
					m_bSoundPause = false;
				}
			}
			else
			{
				//�ꎞ��~����
				m_fAlf = 0.0f;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				if (m_bSoundPause == false)  //���������񂾂��Đ�
				{
					m_bSoundPause = true;
				}

			}
		}
	}
	//�ꎞ��~���j���[�̑I��
	if (m_bPause&&CFade::Get() == FADE_NONE) {
		//[Enter]�������ꂽ�H
		if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetJoyTrigger(JOYSTICKID1, JOY_BUTTON1)) {
			//�I�𒆂̃��j���[���ڂɂ�蕪��
			switch (m_pPause->GetPauseMenu())
			{
			case PAUSE_MENU_CONTINUE:
				CFade::Out(SCENE_GAME);
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;
			case PAUSE_MENU_SELECT:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				CFade::Out(SCENE_SELECT);
				break;
			case PAUSE_MENU_RULE:
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				m_fAlf = 1.0f;

				break;
			}
		}
	}

	//�|�X�g�v���Z�X�X�V
	CPostProcess::Update();

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

	//�|�X�g�v���Z�X�`��
	CPostProcess::Draw();

	//�ꎞ��~�`��
	if (m_bPause) {
		m_pPause->Draw();
	}
	
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
