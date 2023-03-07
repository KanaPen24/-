/*=====�w�b�_�R�����g=====
*@�v���O������	�FStaminabar.cpp
*@�����E�T�v	�F�X�^�~�i�o�[�ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/03/06
*@�X�V����	�F�R�����g�ǋL
*@�X�V��	�F�g�c����
*/
#include "StaminaBar.h"
#include "Texture.h"
#include "Polygon.h"
#include "DebugProc.h"

// �O���[�o���ϐ�
namespace {
	LPCTSTR PATH_TEXTURE[MAX_ST] = {
		_T("data/texture/Bar.png"),
	};
	
	const XMFLOAT4 STAMINABAR_COLOR = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.8f);
	const XMFLOAT2 SIZE_XY = XMFLOAT2(50.0f, 50.0f);
	const float STAMINABAR_POS_X = SCREEN_WIDTH / -2;
	const float STAMINABAR_POS_Y = SCREEN_HEIGHT / -2 + (SIZE_XY.y / 2);
	
}

// �ÓI�����o�ϐ�
ID3D11ShaderResourceView* CStaminaBar::m_pTexture[MAX_ST];

// ������
void CStaminaBar::Init(CGame* pScene)
{
	m_pScene = pScene;
	m_bVisible = true;
	m_pPlayer = m_pScene->GetPlayer();
	m_fStamina = m_pPlayer->GetStamina();
}

// �X�V
void CStaminaBar::Update()
{
	if (!m_bVisible) return;

	//�X�^�~�i�����󂯎��
	m_fStamina = m_pPlayer->GetStamina();
	
#ifdef _DEBUG
	if (CInput::GetKeyPress(VK_RIGHT))
		m_fStamina++;
	if (CInput::GetKeyPress(VK_LEFT))
		m_fStamina--;
	
#endif // _DEBUG

}

// �`��
void CStaminaBar::Draw()
{
	if (!m_bVisible) return;

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);

	// �X�^�~�i�o�[�ݒ�
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetFrameSize(0.0f, 1.0f);
	CPolygon::SetColor(STAMINABAR_COLOR);
	CPolygon::SetTexture(m_pTexture[ST_STAMINABAR]);
	CPolygon::SetSize(m_fStamina*8, SIZE_XY.y);
	CPolygon::SetPos(STAMINABAR_POS_X + (m_fStamina / 2), -STAMINABAR_POS_Y);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	//�l���Z�b�g
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// �e�N�X�`���Ǎ�
HRESULT CStaminaBar::LoadTexture()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	ReleaseTexture();
	// �e�N�X�`���̓ǂݍ���
	for (int i = 0; i < MAX_ST; ++i) {
		hr = CreateTextureFromFile(pDevice, PATH_TEXTURE[i], &m_pTexture[i]);
		if (FAILED(hr)) {
			break;
		}
	}
	return hr;
}

// �e�N�X�`�����
void CStaminaBar::ReleaseTexture()
{
	for (int i = 0; i < MAX_ST; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
	}
}
