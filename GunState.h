//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�g�����`�K���j[GunState.h]
// Author :  
//
//=============================================================================
#ifndef _GUNSTATE_H_
#define _GUNSTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class GunState :
	public ItemState
{
private:
	Item * owner_;	// ��Ԃ̏��L��

public:
	GunState(Item *owner);
	~GunState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
