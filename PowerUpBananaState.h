//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�p���[�A�b�v�o�i�i�j[PowerUpBananaState.h]
// Author :  
//
//=============================================================================
#ifndef _POWERUPBANANASTATE_H_
#define _POWERUPBANANASTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PowerUpBananaState :
	public ItemState
{
private:
	Item * owner_;	// ��Ԃ̏��L��
	int cnt;

public:
	PowerUpBananaState(Item *owner);
	~PowerUpBananaState();

	virtual void Update()override;
	virtual void Start()override;
};

#endif
