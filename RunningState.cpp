//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�_�b�V�����j[RunningState.cpp]
// Author : 
//
//=============================================================================
#include "RunningState.h"
#include "JumpState.h"
#include "IdleState.h"
#include "VictoryState.h"
#include "Input.h"
#include "Map.h"
#include "Timer.h"
#include "ParticleManager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
RunningState::RunningState(Player *owner) : owner_(owner)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
RunningState::~RunningState()
{
	owner_ = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void RunningState::Update(int AnimCurtID)
{	
	// ��ԕω�
	// �S�[���ɓ��B�����̂ŃK�b�c�|�[�Y
	if (owner_->GetPos().x >= GOAL_POS.x)
	{
		owner_->ChangeAnim(Victory);
		owner_->ChangeState(new VictoryState(owner_));
		return;
	}

	// ��L�[�������ăW�����v
	if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(owner_->GetCtrlNum(), BUTTON_B)
		|| owner_->GetAIAction() == eActJump)
	{
		float value = owner_->GetJumpValue();
		owner_->SetJumpSpeed(JUMP_SPEED * value);
		owner_->ChangeAnim(Jump);
		owner_->ChangeState(new JumpState(owner_));

		// PlaySound(�W�����v��)
		return;
	}

	// �O���I�u�W�F�N�g�Ƀq�b�g���Ă���̂őҋ@��Ԃ�
	if (owner_->GetHitHorizon())
	{
		owner_->ChangeAnim(Idle);
		owner_->ChangeState(new IdleState(owner_));
		return;
	}

	// �G�t�F�N�g
	owner_->RunningEffect();
}