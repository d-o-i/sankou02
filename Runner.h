//=============================================================================
//
// �����i�[ [Runner.cpp]
// Author :  
//
//=============================================================================
#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "_2dobj.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Runner :
	public _2dobj
{
private:
	void MakeVertex();
	void SetVertex();

public:
	Runner();
	~Runner();

	void Update()override;
	void Draw()override;
};

#endif
