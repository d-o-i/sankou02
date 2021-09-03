//=============================================================================
//
// �L�����N�^�[�Z���N�g�t���[�� [CharSelectFrame.h]
// Author :  
//
//=============================================================================
#ifndef _CharSelectFrame_H_
#define _CharSelectFrame_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CharSelectFrame : public _2dobj
{
private:
	int Alpha = 192;
	bool AlphaPlus = false;
	void SetAlpha(int Alpha);
	HRESULT MakeVertex();

public:
	CharSelectFrame(D3DXVECTOR2 Pos);
	~CharSelectFrame();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	void SetUse(bool Flag) { this->use = Flag; };
};


#endif