/*=====�w�b�_�R�����g=====
*@�v���O������	�FPlayer.cpp
*@�����E�T�v	�F�v���C���[�̃��f���y�ы����ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2022/06/04
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "Player.h"
#include "Input.h"
#include "Debugproc.h"
#include "GameCamera.h"
#include "Scene.h"
#include <time.h>

// �萔��`
namespace {
	enum EAnimNo {
		ANIM_IDLE = 0,
		ANIM_WALK,
		ANIM_RUN,
		MAX_ANIMNO
	};
}

const float RATE_MOVE_MODEL = 0.25f;			// �ړ������W��
const float SPEED = 0.02f;						// �ړ��X�s�[�h
const float SCALE = 1.0f;						// �傫��
const float DASH = 1.5f;						// �_�b�V��
const float ROTATE = 1.7f;		// �J�����̋���
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	// ���X�e�B�b�N�̃f�b�h�]�[��
//#define REV_Z_AXIS	// Y��180�x��]

// �R���X�g���N�^
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_PLAYER;
	m_uTick = 0;
	m_nSpeed = 0;
	m_pCity = nullptr;
	m_bCollision = false;
	m_pGCam = nullptr;
	XMStoreFloat4x4(&m_mInvWorld, XMMatrixIdentity());
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
}

// ������
HRESULT CPlayer::Init()
{
	HRESULT hr = CModel::Init();
	if (SUCCEEDED(hr)) {
		SetModel(MODEL_PLAYER);
		m_uTick = 0;
		m_pCity = (CCity*)m_pScene->Find(GOT_CITY);
		m_pGCam = CCamera::Get();
		m_naAnimNo = ANIM_IDLE;
		m_dAnimTime = 0.0;
	}
	CAssimpModel* pModel = GetAssimp(MODEL_ENEMY);
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	SetScale(XMFLOAT3(SCALE, SCALE, SCALE));
	SetAngle(XMFLOAT3(0.0f, 180.0f, 0.0f));	//�����ŉ�������
	return hr;
}



// �X�V
void CPlayer::Update()
{
	//�ϐ��錾
	float fDash = 1.0f;
	float fLength = m_pGCam->GetLength();
	CModel::Update();
	XMFLOAT3 vVec = m_pGCam->GetVec();
	vVec.y = 0.0f;

	// ���݈ʒu�擾
	XMFLOAT3 vPos = GetPos();
	m_nSpeed = 0;
	// ��]
	XMFLOAT3 angle = GetAngle();
	// �J�����̌����擾
	XMFLOAT3 rotCamera = m_pGCam->GetAngle();
	//�_�b�V������
	if (CInput::GetKeyPress(VK_LSHIFT) || CInput::GetJoyButton(JOYSTICKID1, JOY_BUTTON4)) fDash = DASH;
	//�Q�[���R���g���[���[
	DWORD JoyCount = CInput::GetJoyCount();
	CInput::GetJoyState(JOYSTICKID1);
	LONG JoyX = CInput::GetJoyX(JOYSTICKID1);
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	if ((JoyX <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyX > -GAMEPAD_LEFT_STICK_DEADZONE) &&
		(JoyY<  GAMEPAD_LEFT_STICK_DEADZONE &&
			JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyX = 0;
		JoyY = 0;
	}
	/*��������R���g���[���[����̓���*/
	if (JoyCount >= 1) {
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE) {
			// �O�ړ�
			vPos.x -= vVec.x* SPEED * fDash;
			vPos.z -= vVec.z* SPEED * fDash;
			m_nSpeed = 1;
		}
		else if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE) {
			// ��ړ�
			vPos.x += vVec.x* SPEED * fDash;
			vPos.z += vVec.z* SPEED * fDash;
			m_nSpeed = 1;
		}
	}

	// �^�C�}�X�V
	++m_uTick;

	//�L�[���͏���
	if (CInput::GetKeyPress(VK_UP)|| CInput::GetKeyPress(VK_W)) {
		// �O�ړ�
		vPos.x -= vVec.x* SPEED * fDash;
		vPos.z -= vVec.z* SPEED * fDash;
		m_nSpeed = 1;
	}
	else if (CInput::GetKeyPress(VK_DOWN)|| CInput::GetKeyPress(VK_S)) {
		// ��ړ�
		vPos.x += vVec.x* SPEED * fDash;
		vPos.z += vVec.z* SPEED * fDash;
		m_nSpeed = 1;
	}
	if (CInput::GetKeyPress(VK_LSHIFT) || CInput::GetJoyButton(JOYSTICKID1, JOY_BUTTON4)) m_nSpeed = 2;

	//�p�x�̕ύX
	if (CInput::GetKeyPress(VK_J))
		angle.y += ROTATE;
	if (CInput::GetKeyPress(VK_L))
		angle.y -= ROTATE;

	// ��]�}�g���b�N�X����
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, XMMatrixRotationY(XMConvertToRadians(angle.y)));
	// ���f�����W���擾
	XMFLOAT3 vZ = XMFLOAT3(mW._31, mW._32, mW._33);

	// �n�ʂƂ̓����蔻��
	XMFLOAT3 vP0;
	vP0.x = vPos.x;
	vP0.y = vPos.y + GetRadius() * 2;
	vP0.z = vPos.z;
	XMFLOAT3 vX, vN;	//��_���W
	if (m_pCity && m_pCity->Collision(vP0,XMFLOAT3(0.0f, -1.0f, 0.0f),m_bCollision, &vX, &vN)) {
		vPos.x = vX.x*0.9f;	
		vPos.z = vX.z;
		m_bCollision = false;	//���������u�Ԃ̍��W���~�����̂ł����œ����蔻�������
	} else {
		vPos.y = 0.0f;
		m_bCollision = true;	//���ꂽ�玟�ɓ�����u�Ԕ����ē����蔻���t����
	}

	//�ړ����f
	mW._41 = vPos.x;
	mW._42 = vPos.y;
	mW._43 = vPos.z;
	SetPos(vPos);

	// ���[���h�ϊ��s��X�V
	SetWorld(mW);

	// �p�x�X�V
	SetAngle(angle);
	
	// �A�j���[�V�����X�V
	if (m_nSpeed == 0)
		m_naAnimNo = ANIM_IDLE;
	if (m_nSpeed == 1)
		m_naAnimNo = ANIM_WALK;
	if (m_nSpeed == 2)
		m_naAnimNo = ANIM_RUN;
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	m_dAnimTime += dSlice;
	if (m_dAnimTime >= GetModel()->GetAnimDuration(m_naAnimNo)) {
		m_dAnimTime = 0.0;
	}
#ifdef _DEBUG
	//�f�o�b�N�\��
	CDebugProc::Print("SPEED * fDash=%f\n", m_nSpeed);
	CDebugProc::Print("Animation=%d\n", m_naAnimNo);
	CDebugProc::Print("��ڲ԰Pos=X:%f Y:%f Z:%f\n", vPos.x, vPos.y, vPos.z);
	CDebugProc::Print("��ڲ԰Vec=X:%f Y:%f Z:%f\n", vVec.x, vVec.y, vVec.z);
	
#endif
}
// �`��
void CPlayer::Draw()
{
	ID3D11Device* pDevice = GetDevice();
	CAssimpModel::InitShader(pDevice, 1);
	CAssimpModel* pModel = GetModel();
	if (pModel) {
		pModel->SetAnimIndex(m_naAnimNo);
		pModel->SetAnimTime(m_dAnimTime);
#ifdef REV_Z_AXIS
		ID3D11DeviceContext* pDC = GetDeviceContext();
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW,
			XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)),
				XMLoadFloat4x4(&GetWorld())));
		pModel->Draw(pDC, mW, eOpacityOnly);
#else
		CModel::Draw();
#endif
	}
}

// �����������`��
void CPlayer::DrawAlpha()
{
	CAssimpModel* pModel = GetModel();
	if (pModel) {
		pModel->SetAnimIndex(m_naAnimNo);
		pModel->SetAnimTime(m_dAnimTime);
#ifdef REV_Z_AXIS
		ID3D11DeviceContext* pDC = GetDeviceContext();
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW,
			XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)),
				XMLoadFloat4x4(&GetWorld())));
		pModel->Draw(pDC, mW, eTransparentOnly);
#else
		CModel::DrawAlpha();
#endif
	}
	CGameObj::DrawAlpha();
}

