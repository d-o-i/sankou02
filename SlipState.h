//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�X���b�v�j[SlipState.h]
// Author :  
//
//=============================================================================
#ifndef _SLIPSTATE_H_
#define _SLIPSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class SlipState :
	public PlayerState
{
private:
	Player * owner_;	// ��Ԃ̏��L��

public:
	SlipState(Player* owner);
	~SlipState();

	virtual void Update(int AnimCurtID)override;
};

#endif
