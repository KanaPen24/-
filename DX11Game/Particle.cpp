/*=====ヘッダコメント=====
*@プログラム名	：Particle.cpp
*@説明・概要	：パーティクルについて
*@製作者	：吉田叶聖
*@更新内容	：2023/01/10
*@更新履歴	：評価課題で作成
*@更新者	：吉田叶聖
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_PARTICLE	L"data/texture/hokori.jpg"	// 読み込むテクスチャファイル名
const float VALUE_MOVE_PARTICLE = 2.0f;					// 移動速度

const int MAX_PARTICLE = 25;							// エフェクト最大数

//#define DISP_SHADOW										// 影の表示
//#undef DISP_SHADOW

const XMFLOAT4 MATERIAL_DIFFUSE = XMFLOAT4(1, 1, 1, 1);
const XMFLOAT4 MATERIAL_AMBIENT = XMFLOAT4(0, 0, 0, 1);
const XMFLOAT4 MATERIAL_SPECULAR = XMFLOAT4(0, 0, 0, 1);
const XMFLOAT4 MATERIAL_EMISSIVE = XMFLOAT4(0, 0, 0, 1);
const float MATERIAL_POWER = 1.0f;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TParticle {
	XMFLOAT3 pos;			// 位置
	XMFLOAT3 rot;			// 回転
	XMFLOAT3 move;			// 移動量
	XMFLOAT4 col;			// 色
	XMFLOAT2 size;			// 幅/高さ
	int nIdxShadow;			// 影ID
	int life;				// 寿命
	bool use;				// 使用しているかどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(ID3D11Device* pDevice);
void SetVertexParticle(int nIdxParticle, XMFLOAT2 size);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_meshParticle;					// メッシュ情報
static MATERIAL			g_material;						// マテリアル

static TParticle		g_particle[MAX_PARTICLE];		// パーティクル情報
static XMFLOAT3			g_posBase;						// パーティクル発生位置
static XMFLOAT2			g_sizeBase = XMFLOAT2(1.0f, 0.1f);	// 基準の横/縦速さ
static bool				g_bPause = false;				// ポーズON/OFF

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	// テクスチャの読み込み
	CreateTextureFromFile(pDevice,					// デバイス ポインタ
						  TEXTURE_PARTICLE,			// ファイル名
						  &g_meshParticle.pTexture);// 読み込むメモリ
	XMStoreFloat4x4(&g_meshParticle.mtxTexture, XMMatrixIdentity());

	// マテリアルの設定
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
// 終了処理
//=============================================================================
void UninitParticle(void)
{
#ifdef DISP_SHADOW
	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		if (g_particle[nCnt].use) {
			// 影削除
			//ReleaseShadow(g_particle[nCnt].nIdxShadow);
			g_particle[nCnt].nIdxShadow = -1;
			g_particle[nCnt].use = false;
			
		}
	}
#endif
	// メッシュの開放
	ReleaseMesh(&g_meshParticle);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	XMFLOAT3 rotCamera;

	// カメラの回転を取得
	rotCamera = CCamera::Get()->GetAngle();

	if (CInput::GetKeyPress(VK_A)) {
		if (CInput::GetKeyPress(VK_W)) {
			// 左前移動
			g_posBase.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PARTICLE;
		} else if (CInput::GetKeyPress(VK_S)) {
			// 左後移動
			g_posBase.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PARTICLE;
		} else {
			// 左移動
			g_posBase.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PARTICLE;
		}
	} else if (CInput::GetKeyPress(VK_D)) {
		if (CInput::GetKeyPress(VK_W)) {
			// 右前移動
			g_posBase.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PARTICLE;
		} else if (CInput::GetKeyPress(VK_S)) {
			// 右後移動
			g_posBase.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PARTICLE;
		} else {
			// 右移動
			g_posBase.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PARTICLE;
			g_posBase.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PARTICLE;
		}
	} else if (CInput::GetKeyPress(VK_W)) {
		// 前移動
		g_posBase.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PARTICLE;
		g_posBase.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PARTICLE;
	} else if (CInput::GetKeyPress(VK_S)) {
		// 後移動
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
				// 使用中
				g_particle[nCnt].pos.x += g_particle[nCnt].move.x;
#ifdef DISP_SHADOW
				// 影の位置設定
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
						// α値設定
						g_particle[nCnt].col.w -= 0.01f;
					}
					// 色の設定
					SetColorParticle(nCnt, g_particle[nCnt].col);
				}
			}
		}

		// パーティクル発生
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
			// ビルボードの設定
			SetParticle(pos, move, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f), size, life);
		}
	}

	// リセット
	if (CInput::GetKeyRelease(VK_RETURN)) {
		g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_sizeBase = XMFLOAT2(5.0f, 10.0f);
	}
#ifdef _DEBUG
	CDebugProc::Print("*** ﾊﾟｰﾃｨｸﾙ ***\n");
	CDebugProc::Print("[ｷｼﾞｭﾝ ｿｸﾄﾞ: (%.1f, %.1f)]\n", g_sizeBase.x, g_sizeBase.y);
	CDebugProc::Print("[ﾎﾟｼﾞｼｮﾝ: (%.1f, %.1f, %.1f)]\n", g_posBase.x, g_posBase.y, g_posBase.z);
	CDebugProc::Print("\n");
	CDebugProc::Print("*** ﾊﾟｰﾃｨｸﾙ ｿｳｻ ***\n");
	CDebugProc::Print("ﾏｴ   ｲﾄﾞｳ : [\x1e]\n");//↑
	CDebugProc::Print("ｳｼﾛ  ｲﾄﾞｳ : [\x1f]\n");//↓
	CDebugProc::Print("ﾋﾀﾞﾘ ｲﾄﾞｳ : [\x1d]\n");//←
	CDebugProc::Print("ﾐｷﾞ  ｲﾄﾞｳ : [\x1c]\n");//→
	CDebugProc::Print("ｷｼﾞｭﾝ ﾖｺ  : [1][2]\n");
	CDebugProc::Print("ｷｼﾞｭﾝ ﾀﾃ  : [3][4]\n");
	CDebugProc::Print("\n");
#endif // DEBUG
	
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	SetBlendState(BS_ADDITIVE);	// 加算合成
	SetZWrite(false);
	CLight::Get()->SetDisable();

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int nCnt = 0; nCnt < MAX_PARTICLE; ++nCnt) {
		if (g_particle[nCnt].use) {
			// ワールドマトリックスの初期化
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

			// スケールを反映
			mtxScale = XMMatrixScaling(g_particle[nCnt].size.x, g_particle[nCnt].size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_particle[nCnt].pos.x, g_particle[nCnt].pos.y, g_particle[nCnt].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			// カラーの設定
			g_material.Diffuse = g_particle[nCnt].col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_meshParticle);
		}
	}

	CLight::Get()->SetEnable();
	SetZBuffer(true);
	SetBlendState(BS_NONE);	// 半透明合成無
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_meshParticle.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshParticle.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);

	// 法線の設定
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);

	// インデックス配列を生成
	g_meshParticle.nNumIndex = 4;
	int* pIndexWk = new int[g_meshParticle.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshParticle, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, XMFLOAT2 size)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_particle[nIdxParticle].size = size;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_particle[nIdxParticle].col = col;
	}
}

//=============================================================================
// エフェクトの設定
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

			// 頂点座標の設定
			SetVertexParticle(nCnt, size);

			// 頂点カラーの設定
			SetColorParticle(nCnt, col);

			nIdxParticle = nCnt;

#ifdef DISP_SHADOW
			// 影の設定
			//g_particle[nCnt].nIdxShadow = CreateShadow(pos, size.x * 0.5f);
#endif

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// 発生位置取得
//=============================================================================
XMFLOAT3 GetParticlePos()
{
	return g_posBase;
}
