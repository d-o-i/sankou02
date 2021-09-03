//=============================================================================
//
// プレイヤーの状態管理（ガッツポーズ中）[VictoryState.h]
// Author :  
//
//=============================================================================
#ifndef _VICTORYSTATE_H_
#define _VICTORYSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class VictoryState :
	public PlayerState
{
private:
	Player * owner_;

public:
	VictoryState(Player * owner);
	~VictoryState();

	virtual void Update(int AnimCurtID)override;
};

#endif
