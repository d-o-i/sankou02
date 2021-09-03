//=============================================================================
//
// 矢印 [Arrow.h]
// Author :   
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Arrow :
	public _2dobj
{
private:
	bool reverse;
	bool draw;
	int cntFlash;
	bool flash;		// 点滅させるフラグ(true = 表示)

	void MakeVertex();

public:
	Arrow(bool reverse);
	~Arrow();

	void Update()override;
	void Draw()override;
};

#endif
