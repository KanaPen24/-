/*=====�w�b�_�R�����g=====
*@�v���O������	�FPostProcess.cpp
*@�����E�T�v	�F�|�X�g�v���Z�X����
*@�����	�F�g�c����
*@�X�V��	�F�g�c����
*@�X�V���e	�F��������
*@�X�V����	�F2023/02/02
*/
#include "PostProcess.h"
#include "Polygon.h"
#include "Sound.h"
#include "Input.h"
#include "DebugProc.h"

// �ÓI�����o
XMFLOAT4 CPostProcess::m_vColor = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);
float CPostProcess::m_fRate = 1.0f / (60.0f * 0.5f);

// ������
void CPostProcess::Init()
{
	m_vColor.w = 0.0f;			// �s�����x
}

// �X�V
void CPostProcess::Update()
{
	if(CInput::GetKeyPress(VK_B))
	m_vColor.w += m_fRate;	// �s�����x�𑝉�
	if (CInput::GetKeyPress(VK_M))
	m_vColor.w -= m_fRate;	// �s�����x��������

	//�ő�l�ƍŏ��l��ݒ�
	m_vColor.w = std::fmax(m_vColor.w, 0.0f);
	m_vColor.w = std::fmin(m_vColor.w, 0.5f);

#ifdef _DEBUG
	CDebugProc::Print("Alf%2f\n", m_vColor.w);
#endif // _DEBUG



	
}

// �`��
void CPostProcess::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);

	CPolygon::SetColor(m_vColor);
	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetPos(0.0f, 0.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// �t�F�[�h�C�� �A�E�g �J���[�ݒ�
void CPostProcess::SetColor(XMFLOAT3 vColor)
{
	m_vColor.x = vColor.x;
	m_vColor.y = vColor.y;
	m_vColor.z = vColor.z;
}

