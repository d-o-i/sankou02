//=============================================================================
//
// �t�B�j�b�V��UI [Finish.h]
// Author :  
//
//=============================================================================
#ifndef _FINISH_H_
#define _FINISH_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Finish :
	public _2dobj
{
private:
	HRESULT MakeVertex();
	void SetVertex();

public:
	Finish();
	~Finish();

	void Update()override;
	void Draw()override;
};

#endif
