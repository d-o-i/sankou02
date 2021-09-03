//=============================================================================
//
// アイテムの状態管理（スパイクブーツ）[SpikeState.h]
// Author :  
//
//=============================================================================
#ifndef _SPIKESTATE_H_
#define _SPIKESTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class SpikeState :
	public ItemState
{
private:
	Item * owner_;
	int cnt;

public:
	SpikeState(Item *owner);
	~SpikeState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
