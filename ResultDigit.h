//=============================================================================
//
// リザルト画面での桁 [ResultDigit.h]
// Author :  
//
//=============================================================================
#ifndef _RESULTDIGIT_H_
#define _RESULTDIGIT_H_

#include "Digit.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class ResultDigit :
	public Digit
{
private:
	int rank;

public:
	ResultDigit(DWORD _time, int _digit, int _rank); // time = その桁の数字、digit = 何桁目か、rank = 順位
	~ResultDigit();

	void Update()override;
	void Draw()override;
};

#endif
