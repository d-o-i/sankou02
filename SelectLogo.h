//=============================================================================
//
// �L�����N�^�[�Z���N�g���S [SelectLogo.h]
// Author :  
//
//=============================================================================
#ifndef _SELECTLOGO_H_
#define _SELECTLOGO_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SelectLogo :
	public _2dobj {
public:
	SelectLogo();
	~SelectLogo();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex();
};


#endif