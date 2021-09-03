//=============================================================================
//
// 3D�r���{�[�h�G�t�F�N�g���� [Effect3D.h]
// Author :  
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "Billboard.h"
#include "Effect.h"

// �G�t�F�N�g�̎��
enum Effect3DNum {
	HitEffect3D,
	DeadEffect3D,
	RunEffect3D,
	ExplosionEffect3D,
	ItemEffect3D,
	Item1Effect3D,
	ChargeEffect3D,

	// �G�t�F�N�g�̌�
	EffectMax3D,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Effect3D :
	public Billboard
{
private:
	bool						use;
	int							CountAnim;
	int							PatternAnim;			// �A�j���[�V�����p�^�[��
	int							AnimationCnt;
	int							TexNo;
	int							TexAnimNum;				// �A�j���[�V�����̕�����
	int							xPattern, yPattern;		// ������
	D3DXVECTOR3					size;					// �\���T�C�Y
	int							loopnum;				// ���[�v�������(�������[�v��-1)
	int							loopcnt;				// ���݂̃��[�v��

	HRESULT MakeVertex();
	void SetTexture();

	void Loop();

public:
	Effect3D(int Effect3DNum, D3DXVECTOR3 _pos, int _LoopNum);
	~Effect3D();

	void Update()override;
	void Draw()override;

	bool GetUse() { return this->use; };
	void SetUse(bool use) { this->use = use; };
};

#endif
