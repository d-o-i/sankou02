//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�ҋ@���j[IdleState.h]
// Author :  
//
//=============================================================================
#ifndef _IDLESTATE_H_
#define _IDLESTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class IdleState :
	public PlayerState
{
private:
	Player * owner_;	// ��Ԃ̏��L��

public:
	IdleState(Player* owner);
	~IdleState();

	virtual void Update(int AnimCurtID)override;
};

#endif
