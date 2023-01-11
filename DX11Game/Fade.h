// �t�F�[�h �C��/�A�E�g [Fade.h]
#pragma once
#include "Main.h"
#include "Scene.h"

// �萔��`
enum EFade {
	FADE_NONE = 0,	// �������Ă��Ȃ�
	FADE_IN,		// �t�F�[�h�C������
	FADE_OUT,		// �t�F�[�h�A�E�g����

	MAX_FADE
};

// �N���X��`
class CFade {
private:
	static EFade m_fade;
	static EScene m_sceneNext;
	static XMFLOAT4 m_vColor;
	static float m_fRate;

public:
	static void Init();
	static void Update();
	static void Draw();

	static void Out(EScene scene, float fSecond = 0.5f);
	static EFade Get() { return m_fade; }
	static void SetColor(XMFLOAT3 vColor);
	static void CutOut(EScene eNext);
};