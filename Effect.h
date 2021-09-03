//=============================================================================
//
// �G�t�F�N�g���� [Effect.h]
// Author :  
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

// �G�t�F�N�g�̎��
enum EffectNum{
	HitEffect,
	DeadEffect,
	RunEffect,
	ExplosionEffect,
	ItemEffect,
	Item1Effect,
	ChargeEffect,

	// �G�t�F�N�g�̌�
	EffectMax,
};

#define INFINITY_LOOP	(-1)	// �G�t�F�N�g�𖳌����[�v������

#include "_2dobj.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Effect :
	public _2dobj
{
private:
	LPDIRECT3DVERTEXBUFFER9		D3DVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					rot;
	D3DXVECTOR3					scl;
	int							CountAnim;
	int							AnimationCnt;
	int							TexNo;
	int							TexAnimNum;				// �A�j���[�V�����̕�����
	int							xPattern, yPattern;		// ������
	int							loopnum;				// ���[�v�������(�������[�v��-1)
	int							loopcnt;				// ���݂̃��[�v��

	HRESULT MakeVertex();						// ���_���̍쐬
	void SetTexture(int cntPattern);			// �e�N�X�`�����W�̐ݒ�
	void SetVertex();							// ���_���W�̐ݒ�

	void Loop();								// �G�t�F�N�g�̃��[�v����

public:
	Effect(EffectNum num, D3DXVECTOR3 _pos, int _LoopNum);
	~Effect();

	void Update();
	void Draw();

	bool GetUse() { return use; };
	void SetUse(bool _use) { use = _use; };
};

#endif

