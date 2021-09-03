//=============================================================================
//
// チュートリアル画面 [Tutorial.h]
// Author :  
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Tutorial :
	public _2dobj
{
private:
	void MakeVertex();
	void SetVertex();
	void SetTexture();

	static int slideNo;
	D3DXVECTOR3 move;
	D3DXVECTOR3 newPos;
	int moveFrame;

	void Debug();

public:
	Tutorial();
	~Tutorial();

	void Update()override;
	void Draw()override;

	static int GetSlideNo() { return slideNo; };
};

#endif
