//=============================================================================
//
// プレイヤーの状態管理（待機中）[IdleState.h]
// Author :  
//
//=============================================================================
#ifndef _IDLESTATE_H_
#define _IDLESTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class IdleState :
	public PlayerState
{
private:
	Player * owner_;	// 状態の所有者

public:
	IdleState(Player* owner);
	~IdleState();

	virtual void Update(int AnimCurtID)override;
};

#endif
