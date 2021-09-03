//=============================================================================
//
// �A�C�e������ [Item.h]
// Author :  
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "_2dobj.h"
#include "Player.h"
#include "ItemState.h"
#include "Effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_SIZE				D3DXVECTOR2(50.0f, 50.0f)
#define ROULETTE_COUNTER		(10)						// ���t���[�����ƂɃ��[���b�g�𓮂�����
#define USE_COUNTER				(3)							// ���񃋁[���b�g�𓮂�������A�C�e�����m�肳���邩
#define DIVIDE_ITEM_X			(7)
#define DIVIDE_ITEM_Y			(1)

enum ItemNum
{
	NumJet,		// �W�F�b�g�p�b�N
	NumSpike,	// �X�p�C�N�u�[�c
	NumPowerUp,	// �p���[�A�b�v�o�i�i
	NumKawa,	// �o�i�i�̔�
	NumBlind,	// �u���C���h
	NumSpInk,	// �X�y�V�����C���N
	NumGun,		// �g�����`�K��

	NumItemMax,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Item :
	public _2dobj
{
private:
	Effect *effect;
	ItemState *state[NumItemMax];			// �X�e�[�^�X���ۃN���X
	Player *pPlayer;						// �Q�Ƃ���v���C���[�N���X�̃|�C���^
	int rouletteCnt;						// ���[���b�g�̃J�E���^
	int useCnt;								// ���[���b�g�ŃA�C�e���̃e�N�X�`�����񂷂��߂̃J�E���^
	bool active;							// �A�C�e���g�p��

	HRESULT MakeVertex();
	void SetTexture();
	void SetVertex();

	void Start();
	void ActiveState(int ItemID);
	void Debug();

public:
	Item(Player* ptr);
	~Item();

	void Update();
	void Draw();

	void ChangeState(int ItemID);
	void Reset();

	bool GetUse() { return use; };
	Player *GetPlayer() { return pPlayer; };
	Effect *GetEffect() { return effect; };

	void SetPatternAnim(int Anim) { PatternAnim = Anim; };
};

#endif
