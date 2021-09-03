//=============================================================================
//
// フィニッシュUI [Finish.h]
// Author :  
//
//=============================================================================
#ifndef _FINISH_H_
#define _FINISH_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
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
