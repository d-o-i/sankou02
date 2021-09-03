//=============================================================================
//
// プレイヤーの状態管理（拍手中）[ClappingState.h]
// Author :  
//
//=============================================================================
#ifndef _CLAPPINGSTATE_H_
#define _CLAPPINGSTATE_H_

#include "PlayerState.h"
#include "Player.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class ClappingState :
	public PlayerState
{
private:
	Player * owner_;

public:
	ClappingState(Player* owner);
	~ClappingState();

	virtual void Update(int AnimCurtID);
};

#endif