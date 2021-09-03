//=============================================================================
//
// プレイヤーの状態管理（待機中）[IdleState.cpp]
// Author : 
//
//=============================================================================
#include "IdleState.h"
#include "RunningState.h"
#include "VictoryState.h"
#include "JumpState.h"
#include "Input.h"
#include "Player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
IdleState::IdleState(Player* owner) : owner_(owner)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
IdleState::~IdleState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void IdleState::Update(int AnimCurtID)
{
	// 状態変化
	if (owner_->GetPlayable())
	{
		// 前方オブジェクトにあたっていない場合、ダッシュに移行
		if (!owner_->GetHitHorizon())
		{
			owner_->ChangeAnim(Running);
			owner_->ChangeState(new RunningState(owner_));
			return;
		}
		else
		{
			// Uキーを押してジャンプ
			if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(owner_->GetCtrlNum(), BUTTON_B) 
				|| owner_->GetAIAction() == eActJump)
			{
				float value = owner_->GetJumpValue();
				owner_->SetJumpSpeed(JUMP_SPEED * value);
				owner_->ChangeAnim(Jump);
				owner_->ChangeState(new JumpState(owner_));

				// PlaySound(ジャンプ音)
				return;
			}
		}
	}
}