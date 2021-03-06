//=============================================================================
//
// アイテムの状態管理（ジェットパック）[JetState.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "JetState.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ACTIVE_TIME	(600)	// 有効時間

//=============================================================================
// コンストラクタ
//=============================================================================
JetState::JetState(Item *owner) : owner_(owner)
{
	cnt = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
JetState::~JetState()
{
	owner_ = nullptr;
}

//=============================================================================
// 更新
//=============================================================================
void JetState::Update()
{
	cnt++;

	if (cnt == ACTIVE_TIME)
	{
		owner_->GetPlayer()->SetJet(false);
		// アイテムの効果終了
		owner_->Reset();
	}

	// プレイヤーにエフェクト発生
	owner_->GetPlayer()->PowwrUpEffect();

}

//=============================================================================
// 有効にするときだけ使う
//=============================================================================
void JetState::Start()
{
	cnt = 0;
	owner_->GetPlayer()->SetJet(true);
}