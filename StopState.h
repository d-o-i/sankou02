//=============================================================================
//
// �v���C���[�̏�ԊǗ��i��~��ԁj[StopState.h]
// Author :  
//
//=============================================================================
#ifndef _STOPSTATE_H_
#define _STOPSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class StopState :
	public PlayerState
{
private:
	Player * owner_;	// ��Ԃ̏��L��
	int cnt;

public:
	StopState(Player* owner);
	~StopState();

	virtual void Update(int AnimID)override;
};

#endif
