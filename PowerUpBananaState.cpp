//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�p���[�A�b�v�o�i�i�j[PowerUpBananaState.h]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "PowerUpBananaState.h"
#include "ParticleManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ACTIVE_TIME	(600)	// �L������

//=============================================================================
// �R���X�g���N�^
//=============================================================================
PowerUpBananaState::PowerUpBananaState(Item *owner) : owner_(owner)
{
	cnt = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
PowerUpBananaState::~PowerUpBananaState()
{
	owner_ = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void PowerUpBananaState::Update()
{
	cnt++;

	// �L�����Ԃ��I��
	if (cnt == ACTIVE_TIME)
	{
		owner_->GetPlayer()->SetPowerBanana(false);
		//owner_->GetPlayer()->SetJumpValue(1.0f);
		//owner_->GetPlayer()->SetRunSpd(1.0f);

		// �A�C�e�����g�p
		owner_->SetPatternAnim(NumKawa);
		owner_->ChangeState(NumKawa);
	}

	// �v���C���[�ɃG�t�F�N�g����
	if (owner_->GetPlayer()->GetAnimCurtID() == Running || owner_->GetPlayer()->GetAnimCurtID() == Idle || owner_->GetPlayer()->GetAnimCurtID() == Jump)
	{
		owner_->GetPlayer()->PowwrUpEffect();
	}
}

//=============================================================================
// �L���ɂ���Ƃ������g��
//=============================================================================
void PowerUpBananaState::Start()
{
	cnt = 0;

	//owner_->GetPlayer()->SetJumpValue(2.0f);
	//owner_->GetPlayer()->SetJumpValue(2.0f);
	owner_->GetPlayer()->SetPowerBanana(true);
}