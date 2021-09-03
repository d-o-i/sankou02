//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�u���C���h�j[BlindState.h]
// Author :
//
//=============================================================================
#include "Main.h"
#include "BlindState.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ACTIVE_TIME	(420)	// �L������

//=============================================================================
// �R���X�g���N�^
//=============================================================================
BlindState::BlindState(Item *owner) : owner_(owner)
{
	cnt = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
BlindState::~BlindState()
{
	owner_ = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void BlindState::Update()
{
	cnt++;

	if (cnt == ACTIVE_TIME)
	{
		// �A�C�e���̌��ʏI��
		owner_->Reset();
		owner_->GetPlayer()->SetBlind(false);
	}
}

//=============================================================================
// �L���ɂ���Ƃ������g��
//=============================================================================
void BlindState::Start()
{
	cnt = 0;
	owner_->GetPlayer()->SetBlind(true);
}