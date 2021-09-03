//=============================================================================
//
// �J�E���g�_�E������ [CountDown.h]
// Author :  
//
//=============================================================================
#ifndef _COWNTDOWN_H_
#define _COUNTDOWN_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CountDown : public _2dobj
{
private:
	int starttimer;
	int startsecond;

	HRESULT MakeVertex();
	void SetTexture();
public:
	CountDown();
	~CountDown();
	void Update();
	void Draw();
};

#endif
