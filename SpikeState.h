//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�X�p�C�N�u�[�c�j[SpikeState.h]
// Author :  
//
//=============================================================================
#ifndef _SPIKESTATE_H_
#define _SPIKESTATE_H_

#include "ItemState.h"
#include "Item.h"

//*****************************************************************************
// �N���X��`
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
