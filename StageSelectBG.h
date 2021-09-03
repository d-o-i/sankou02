//=============================================================================
//
// �X�e�[�W�Z���N�g��ʂ̔w�i [StageSelectBG.h]
// Author :  
//
//=============================================================================
#ifndef _STAGESELECTBG_H_
#define _STAGESELECTBG_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class StageSelectBG :
	public _2dobj
{
private:
	void MakeVertex();
	void SetTexture();
	void SetVertex();

	static int PatternAnim;

public:
	StageSelectBG();
	~StageSelectBG();

	void Update()override;
	void Draw()override;

	static int GetStageSelect() { return PatternAnim; };
};

#endif
