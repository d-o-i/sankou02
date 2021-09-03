//=============================================================================
//
// アイテムの状態管理（ブラインド）[BlindState.h]
// Author : 
//
//=============================================================================
#ifndef _BLINDSTATE_H_
#define _BLINDSTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class BlindState :
	public ItemState
{
private:
	Item * owner_;
	int cnt;

public:
	BlindState(Item *owner);
	~BlindState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
