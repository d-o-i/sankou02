//=============================================================================
//
// �v���C���[�̏�ԊǗ��i�����|�[�Y���j[LoseState.h]
// Author :  
//
//=============================================================================
#ifndef _LOSESTATE_H_
#define _LOSESTATE_H_

#include "PlayerState.h"
#include "Player.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class LoseState :
	public PlayerState
{
private:
	Player * owner_;

public:
	LoseState(Player* owner);
	~LoseState();

	virtual void Update(int AnimCurtID)override;

};

#endif
