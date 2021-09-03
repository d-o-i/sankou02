//=============================================================================
//
// プレイヤーの状態管理（ダッシュ中）[RunningState.cpp]
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
// コンストラクタ
//=============================================================================
RunningState::RunningState(Player *owner) : owner_(owner)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
RunningState::~RunningState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void RunningState::Update(int AnimCurtID)
{	
	// 状態変化
	// ゴールに到達したのでガッツポーズ
	if (owner_->GetPos().x >= GOAL_POS.x)
	{
		owner_->ChangeAnim(Victory);
		owner_->ChangeState(new VictoryState(owner_));
		return;
	}

	// 上キーを押してジャンプ
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

	// 前方オブジェクトにヒットしているので待機状態へ
	if (owner_->GetHitHorizon())
	{
		owner_->ChangeAnim(Idle);
		owner_->ChangeState(new IdleState(owner_));
		return;
	}

	// エフェクト
	owner_->RunningEffect();
}