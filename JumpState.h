//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�W�����v���j[JumpState.h]
// Author :  
//
//=============================================================================
#ifndef _JUMPSTATE_H_
#define _JUMPSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class JumpState :
	public PlayerState
{
private:
	Player * owner_;

public:
	JumpState(Player *owner);
	~JumpState();

	virtual void Update(int AnimCurtID)override;
};

#endif
