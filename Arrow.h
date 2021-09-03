//=============================================================================
//
// ��� [Arrow.h]
// Author :   
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "_2dobj.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Arrow :
	public _2dobj
{
private:
	bool reverse;
	bool draw;
	int cntFlash;
	bool flash;		// �_�ł�����t���O(true = �\��)

	void MakeVertex();

public:
	Arrow(bool reverse);
	~Arrow();

	void Update()override;
	void Draw()override;
};

#endif
