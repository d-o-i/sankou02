//=============================================================================
//
// �|�b�v�A�b�v�\������ [Pop.h]
// Author :  
//
//=============================================================================
#ifndef _POP_H_
#define _POP_H_

#include "Billboard.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Pop : public Billboard
{
private:
	int PlayerNo;

	HRESULT MakeVertex();

public:
	Pop(int PlayerNo);
	~Pop();

	void Update() {};
	void Update(D3DXVECTOR3 PlayerPos);
	void Draw();
};

#endif
