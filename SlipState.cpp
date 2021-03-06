//=============================================================================
//
// プレイヤーの状態管理（スリップ）[SlipState.cpp]
// Author :  
//
//=============================================================================
#include "SlipState.h"
#include "IdleState.h"

//=============================================================================
// コンストラクタ
//=============================================================================
SlipState::SlipState(Player* owner) : owner_(owner)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
SlipState::~SlipState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void SlipState::Update(int AnimCurtID)
{
	// アイドル状態にアニメーションが変わったらステータスも変更する
	if (owner_->GetAnimCurtID() == Idle)
	{
		owner_->ChangeState(new IdleState(owner_));
		owner_->SetPlayable(true);
	}
}