//=============================================================================
//
// ���U���g��� [Result.h]
// Author :  
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class RESULT :
	public _2dobj
{
public:
	RESULT();
	~RESULT();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex(void);
};

#endif
