//=============================================================================
//
// ���U���g��ʂł̌� [ResultDigit.h]
// Author :  
//
//=============================================================================
#ifndef _RESULTDIGIT_H_
#define _RESULTDIGIT_H_

#include "Digit.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ResultDigit :
	public Digit
{
private:
	int rank;

public:
	ResultDigit(DWORD _time, int _digit, int _rank); // time = ���̌��̐����Adigit = �����ڂ��Arank = ����
	~ResultDigit();

	void Update()override;
	void Draw()override;
};

#endif
