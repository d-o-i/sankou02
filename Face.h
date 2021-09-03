//=============================================================================
//
// �L�����N�^�[�̊�\�� [Face.h]
// Author :  
//
//=============================================================================
#ifndef _FACE_H_
#define _FACE_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Face :
	public _2dobj
{
private:
	int playerNo;	// �Q�Ƃ���v���C���[�ԍ�
	int charNo;		// �I�������L�����N�^�[�ԍ�

public:
	Face(int playerNo, int charNo);
	~Face();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture();
	void SetVertex();

};

#endif