//=============================================================================
//
// プレイヤーの状態管理（負けポーズ中）[LoseState.h]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "LoseState.h"

//=============================================================================
// コンストラクタ
//=============================================================================
LoseState::LoseState(Player* owner) : owner_(owner)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
LoseState::~LoseState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void LoseState::Update(int AnimCurtID)
{
}