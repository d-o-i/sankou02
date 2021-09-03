//=============================================================================
//
// �^�C�g���\������ [TitleCursor.h]
// Author :  
//
//=============================================================================
#ifndef _TITLECURSOR_H_
#define _TITLECURSOR_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class TITLECURSOR :
	public _2dobj
{
public:
	TITLECURSOR();
	~TITLECURSOR();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex(void);
	void SetVertexMove(D3DXVECTOR3 pos);

};

#endif 
