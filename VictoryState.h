//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�K�b�c�|�[�Y���j[VictoryState.h]
// Author :  
//
//=============================================================================
#ifndef _VICTORYSTATE_H_
#define _VICTORYSTATE_H_

#include "PlayerState.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class VictoryState :
	public PlayerState
{
private:
	Player * owner_;

public:
	VictoryState(Player * owner);
	~VictoryState();

	virtual void Update(int AnimCurtID)override;
};

#endif
