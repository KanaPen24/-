// 爆発エフェクト クラス [Explosion.h]
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

// 構造体定義
//----- 頂点座標
struct VERTEX {
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
};

// 破片クラス
class CDebris
{
private:
	XMFLOAT3 m_vertex[3];	// 頂点座標
	XMFLOAT3 m_normal;		// 法線ベクトル
	XMFLOAT4X4 m_world;		// ワールド変換
	XMFLOAT3 m_velocity;	// 速度
	XMFLOAT3 m_accel;		// 加速度
	XMFLOAT3 m_axis;		// 回転軸
	float m_rotate;			// 回転速度
	float m_alpha;			// 不透明度

public:
	HRESULT Init(XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2,
		float fRadius, XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity);	// 初期化
	void Update();								// 更新
	void UpdateBuffer(VERTEX* pVertex);			// 頂点バッファ更新
};

// 爆発エフェクト クラス
class CExplosion : public CGameObj
{
private:
	CDebris* m_pDebris;							// 破片
	UINT m_uDebris;								// 破片の数
	ID3D11Buffer* m_pVertexBuffer;				// 頂点バッファ
	static ID3D11Buffer* m_pConstantBuffer[2];	// 定数バッファ
	static ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダ
	static ID3D11InputLayout* m_pInputLayout;	// 頂点フォーマット
	static ID3D11PixelShader* m_pPixelShader;	// ピクセルシェーダ
	TAssimpVertex* m_pVertex;					// 頂点配列
	UINT m_uVertex;								// 頂点数
	UINT* m_pIndex;								// インデックス配列
	UINT m_uIndex;								// インデックス数

public:
	CExplosion(CScene* pScene);
	~CExplosion();
	static HRESULT InitShader();
	static void FinShader();
	HRESULT Init();					// 初期化
	void Fin();						// 終了処理
	void Update();					// 更新
	void Draw();					// 描画
	void DrawAlpha();				// 半透明部分描画
	// モデル設定
	HRESULT SetModel(CAssimpModel* pModel);
	// 爆発開始
	HRESULT Start(XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity = nullptr);
};
