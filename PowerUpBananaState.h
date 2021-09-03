//=============================================================================
//
// アイテムの状態管理（パワーアップバナナ）[PowerUpBananaState.h]
// Author :  
//
//=============================================================================
#ifndef _POWERUPBANANASTATE_H_
#define _POWERUPBANANASTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class PowerUpBananaState :
	public ItemState
{
private:
	Item * owner_;	// 状態の所有者
	int cnt;

public:
	PowerUpBananaState(Item *owner);
	~PowerUpBananaState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
