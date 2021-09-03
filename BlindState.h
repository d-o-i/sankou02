//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�u���C���h�j[BlindState.h]
// Author : 
//
//=============================================================================
#ifndef _BLINDSTATE_H_
#define _BLINDSTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// �N���X��`
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
