//=============================================================================
//
// �X�e�[�W�Z���N�g�p�J�[�\�� [StageSelectCursor.h]
// Author :  
//
//=============================================================================
#ifndef _STAGESELECTCURSOR_H_
#define _STAGESELECTCURSOR_H_

#include "_2dobj.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class StageSelectCursor :
	public _2dobj
{
private:
	void MakeVertex();
	void SetVertex();

public:
	StageSelectCursor();
	~StageSelectCursor();

	void Update()override;
	void Draw()override;
};

#endif
