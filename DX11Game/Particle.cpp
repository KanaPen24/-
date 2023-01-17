/*=====�w�b�_�R�����g=====
*@�v���O������	�FParticle.cpp
*@�����E�T�v	�F�p�[�e�B�N���ɂ���
*@�����	�F�g�c����
*@�X�V���e	�F2023/01/10
*@�X�V����	�F�]���ۑ�ō쐬
*@�X�V��	�F�g�c����
*/
#include "particle.h"
#include "Texture.h"
#include "mesh2.h"
#include "input.h"
#include "Camera.h"
#include "Light.h"
#include "shadow.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PARTICLE	L"data/texture/hokori.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
const float VALUE_MOVE_PARTICLE = 2.0f;					// �ړ����x

const int MAX_PARTICLE = 25;							// �G�t�F�N�g�ő吔

//#define DISP_SHADOW										// �e�̕\��
//#undef DISP_SHADOW

const XMFLOAT4 MATERIAL_DIFFUSE = XMFLOAT4(1, 1, 1, 1);
const XMFLOAT4 MATERIAL_AMBIENT = XMFLOAT4(0, 0, 0, 1);
const XMFLOAT4 MATERIAL_SPECULAR = XMFLOAT4(0, 0, 0, 1);
const XMFLOAT4 MATERIAL_EMISSIVE = XMFLOAT4(0, 0, 0, 1);
const float MATERIAL_POWER = 1.0f;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TParticle {
	XMFLOAT3 pos;			// �ʒu
	XMFLOAT3 rot;			// ��]
	XMFLOAT3 move;			// �ړ���
	XMFLOAT4 col;			// �F
	XMFLOAT2 size;			// ��/����
	int nIdxShadow;			// �eID
	int life;				// ����
	bool use;				// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(ID3D11Device* pDevice);
void SetVertexParticle(int nIdxParticle, XMFLOAT2 size);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_meshParticle;					// ���b�V�����
static MATERIAL			g_material;						// �}�e���A��

static TParticle		g_particle[MAX_PARTICLE];		// �p�[�e�B�N�����
static XMFLOAT3			g_posBase;						// �p�[�e�B�N�������ʒu
static XMFLOAT2			g_sizeBase = XMFLOAT2(1.0f, 0.1f);	// ��̉�/�c����
static bool				g_bPause = false;				// �|�[�YON/OFF

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	ID3D11Device* pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexParticle(pDevice);

	// �e�N�X�`���̓ǂݍ���
	CreateTextureFromFile(pDevice,					// �f�o�C�X �|�C���^
						  TEXTURE_PARTICLE,			// �t�@�C����
						  &g_meshParticle.pTexture);// �ǂݍ��ރ�����
	XMStoreFloat4x4(&g_meshParticle.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshParticle.pMaterial = &g_material;

	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		g_particle[nCnt].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_particle[nCnt].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_particle[nCnt].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_particle[nCnt].size = XMFLOAT2(rand() % 3, rand() % 3);
		g_particle[nCnt].nIdxShadow = -1;
		g_particle[nCnt].life = 0;
		g_particle[nCnt].use = false;
		
	}
	g_posBase= XMFLOAT3(-200.0f, 0.0f, 0.0f);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
#ifdef DISP_SHADOW
	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		if (g_particle[nCnt].use) {
			// �e�폜
			//ReleaseShadow(g_particle[nCnt].nIdxShadow);
			g_particle[nCnt].nIdxShadow = -1;
			g_particle[nCnt].use = false;
			
		}
	}
#endif
	// ���b�V���̊J��
	ReleaseMesh(&g_meshParticle);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	XMFLOAT3 rotCamera;

	// �J�����̉�]���擾
	rotCamera = CCamera::Get()->GetAngle();

	if (CInput::GetKeyPress(VK_A)) {
		if (CInput::GetKeyPress(VK_W)) {
			// ���O�ړ�
			g_posBase.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PARTICLE;
		} else if (CInput::GetKeyPress(VK_S)) {
			// ����ړ�
			g_posBase.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PARTICLE;
		} else {
			// ���ړ�
			g_posBase.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PARTICLE;
		}
	} else if (CInput::GetKeyPress(VK_D)) {
		if (CInput::GetKeyPress(VK_W)) {
			// �E�O�ړ�
			g_posBase.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PARTICLE;
		} else if (CInput::GetKeyPress(VK_S)) {
			// �E��ړ�
			g_posBase.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PARTICLE;
		} else {
			// �E�ړ�
			g_posBase.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PARTICLE;
		}
	} else if (CInput::GetKeyPress(VK_W)) {
		// �O�ړ�
		g_posBase.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PARTICLE;
		g_posBase.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PARTICLE;
	} else if (CInput::GetKeyPress(VK_S)) {
		// ��ړ�
		g_posBase.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PARTICLE;
		g_posBase.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PARTICLE;
	}
	if (CInput::GetKeyPress(VK_Q))
		g_posBase.y++;
	if (CInput::GetKeyPress(VK_E))
		g_posBase.y--;
	if (CInput::GetKeyPress(VK_1) || CInput::GetKeyPress(VK_NUMPAD1)) {
		g_sizeBase.x -= 0.1f;
		if (g_sizeBase.x < 2.0f) {
			g_sizeBase.x = 2.0f;
		}
	}
	if (CInput::GetKeyPress(VK_2) || CInput::GetKeyPress(VK_NUMPAD2)) {
		g_sizeBase.x += 0.1f;
		if (g_sizeBase.x > 10.0f) {
			g_sizeBase.x = 10.0f;
		}
	}
	if (CInput::GetKeyPress(VK_3) || CInput::GetKeyPress(VK_NUMPAD3)) {
		g_sizeBase.y -= 0.1f;
		if (g_sizeBase.y < 5.0f) {
			g_sizeBase.y = 5.0f;
		}
	}
	if (CInput::GetKeyPress(VK_4) || CInput::GetKeyPress(VK_NUMPAD4)) {
		g_sizeBase.y += 0.1f;
		if (g_sizeBase.y > 15.0f) {
			g_sizeBase.y = 15.0f;
		}
	}

	if (CInput::GetKeyTrigger(VK_P) || CInput::GetKeyTrigger(VK_PAUSE)) {
		g_bPause = !g_bPause;
	}

	if (!g_bPause) {
		for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
			if (g_particle[nCnt].use) {
				// �g�p��
				g_particle[nCnt].pos.x += g_particle[nCnt].move.x;
#ifdef DISP_SHADOW
				// �e�̈ʒu�ݒ�
			//	MoveShadow(g_particle[nCnt].nIdxShadow, XMFLOAT3(g_particle[nCnt].pos.x, 0.1f, g_particle[nCnt].pos.z));
#endif
				g_particle[nCnt].life--;
				if (g_particle[nCnt].life <= 0) {
					g_particle[nCnt].use = false;
				//	ReleaseShadow(g_particle[nCnt].nIdxShadow);
					g_particle[nCnt].nIdxShadow = -1;
				} else {
					g_particle[nCnt].col.x = 0.5;
					g_particle[nCnt].col.y = 0.5;
					g_particle[nCnt].col.z = 0.5f;
					if (g_particle[nCnt].life >= 100 && g_particle[nCnt].col.w < 1.0f)
					g_particle[nCnt].col.w += 0.01f;
					else
					{
						// ���l�ݒ�
						g_particle[nCnt].col.w -= 0.01f;
					}
					// �F�̐ݒ�
					SetColorParticle(nCnt, g_particle[nCnt].col);
				}
			}
		}

		// �p�[�e�B�N������
		if((rand() % 2) == 0)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int life;
			XMFLOAT2 size;
			pos.x = -(SCREEN_WIDTH / 2) + rand() % (SCREEN_WIDTH / 2);
			pos.y = rand() % SCREEN_HEIGHT;
			pos.z = -100;
			fAngle = rand() % 360 - 180.0f;
			fLength = rand() % (int)(g_sizeBase.x * 1) / 1.0f - g_sizeBase.x;
			move.x = rand() % (int)(g_sizeBase.x * 2) / 1.0f - g_sizeBase.x;
			move.x = SinDeg(fAngle) * fLength;
			move.x *= 0.1f;
			move.y = SinDeg(fAngle) * fLength;
			life = 100 + rand() % 200;
			size.x = size.y = (float)(rand() % 5 + 10);
			// �r���{�[�h�̐ݒ�
			SetParticle(pos, move, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f), size, life);
		}
	}

	// ���Z�b�g
	if (CInput::GetKeyRelease(VK_RETURN)) {
		g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_sizeBase = XMFLOAT2(5.0f, 10.0f);
	}
#ifdef _DEBUG
	CDebugProc::Print("*** �߰è�� ***\n");
	CDebugProc::Print("[��ޭ� ����: (%.1f, %.1f)]\n", g_sizeBase.x, g_sizeBase.y);
	CDebugProc::Print("[�߼޼��: (%.1f, %.1f, %.1f)]\n", g_posBase.x, g_posBase.y, g_posBase.z);
	CDebugProc::Print("\n");
	CDebugProc::Print("*** �߰è�� ��� ***\n");
	CDebugProc::Print("ϴ   ��޳ : [\x1e]\n");//��
	CDebugProc::Print("���  ��޳ : [\x1f]\n");//��
	CDebugProc::Print("���� ��޳ : [\x1d]\n");//��
	CDebugProc::Print("з�  ��޳ : [\x1c]\n");//��
	CDebugProc::Print("��ޭ� ֺ  : [1][2]\n");
	CDebugProc::Print("��ޭ� ��  : [3][4]\n");
	CDebugProc::Print("\n");
#endif // DEBUG
	
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	SetBlendState(BS_ADDITIVE);	// ���Z����
	SetZWrite(false);
	CLight::Get()->SetDisable();

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		if (g_particle[nCnt].use) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			g_meshParticle.mtxWorld._11 = mtxView._11;
			g_meshParticle.mtxWorld._12 = mtxView._21;
			g_meshParticle.mtxWorld._13 = mtxView._31;
			g_meshParticle.mtxWorld._21 = mtxView._12;
			g_meshParticle.mtxWorld._22 = mtxView._22;
			g_meshParticle.mtxWorld._23 = mtxView._32;
			g_meshParticle.mtxWorld._31 = mtxView._13;
			g_meshParticle.mtxWorld._32 = mtxView._23;
			g_meshParticle.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshParticle.mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(g_particle[nCnt].size.x, g_particle[nCnt].size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_particle[nCnt].pos.x, g_particle[nCnt].pos.y, g_particle[nCnt].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			// �J���[�̐ݒ�
			g_material.Diffuse = g_particle[nCnt].col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_meshParticle);
		}
	}

	CLight::Get()->SetEnable();
	SetZBuffer(true);
	SetBlendState(BS_NONE);	// ������������
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_meshParticle.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshParticle.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �C���f�b�N�X�z��𐶐�
	g_meshParticle.nNumIndex = 4;
	int* pIndexWk = new int[g_meshParticle.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshParticle, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int nIdxParticle, XMFLOAT2 size)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_particle[nIdxParticle].size = size;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_particle[nIdxParticle].col = col;
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int life)
{
	int nIdxParticle = -1;

	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		if (!g_particle[nCnt].use) {
			g_particle[nCnt].pos = pos;
			g_particle[nCnt].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_particle[nCnt].move = move;
			g_particle[nCnt].life = life;
			g_particle[nCnt].use = true;

			// ���_���W�̐ݒ�
			SetVertexParticle(nCnt, size);

			// ���_�J���[�̐ݒ�
			SetColorParticle(nCnt, col);

			nIdxParticle = nCnt;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			//g_particle[nCnt].nIdxShadow = CreateShadow(pos, size.x * 0.5f);
#endif

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// �����ʒu�擾
//=============================================================================
XMFLOAT3 GetParticlePos()
{
	return g_posBase;
}
