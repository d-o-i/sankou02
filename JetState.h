//=============================================================================
//
// アイテムの状態管理（ジェットパック）[JetState.h]
// Author :  
//
//=============================================================================
#ifndef _JETSTATE_H_
#define _JETSTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class JetState :
	public ItemState
{
private:
	Item * owner_;
	int cnt;

public:
	JetState(Item *owner);
	~JetState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
