//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�o�i�i�̔�j[BanananoKawaState.h]
// Author :  
//
//=============================================================================
#ifndef _BANANANOKAWASTATE_H_
#define _BANANANOKAWASTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BanananoKawaState :
	public ItemState
{
private:
	Item * owner_;	// ��Ԃ̏��L��

public:
	BanananoKawaState(Item *owner);
	~BanananoKawaState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
