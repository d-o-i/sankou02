//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�_�b�V�����j[RunningState.h]
// Author :  
//
//=============================================================================
#ifndef _RUNNINGSTATE_H_
#define _RUNNINGSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class RunningState :
	public PlayerState
{
private:
	Player * owner_;

public:
	RunningState(Player *owner);
	~RunningState();

	virtual void Update(int AnimCurtID)override;
};

#endif
