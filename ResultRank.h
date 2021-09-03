//=============================================================================
//
// �v���C���[���� [ResultRank.h]
// Author :  
//
//=============================================================================
#ifndef _RESULTRANK_H_
#define _RESULTRANK_H_

#include "_2dobj.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULTRANK_DIVIDE_X	(1)
#define RESULTRANK_DIVIDE_Y	(4)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ResultRank :
	public _2dobj
{
private:
	int rank;	// ����
	int owner;	// �v���C���[�ԍ�
	HRESULT MakeVertex();

public:
	ResultRank(int rank, int owner);
	~ResultRank();

	void Update();
	void Draw();

};

#endif