//=============================================================================
//
// プレイヤーの状態管理（停止状態）[StopState.cpp]
// Author :  
//
//=============================================================================
#include "StopState.h"
#include "RunningState.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
#define ACTIVE_TIME	(120)

//=============================================================================
// コンストラクタ
//=============================================================================
StopState::StopState(Player* owner) : owner_(owner)
{
	cnt = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
StopState::~StopState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void StopState::Update(int AnimCurtID)
{
	cnt++;

	if (cnt == ACTIVE_TIME)
	{
		cnt = 0;
		owner_->ChangeAnim(Running);
		owner_->SetPlayable(true);
		owner_->ChangeState(new RunningState(owner_));
		return;
	}
}