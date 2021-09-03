//=============================================================================
//
// ステージセレクト用カーソル [StageSelectCursor.h]
// Author :  
//
//=============================================================================
#ifndef _STAGESELECTCURSOR_H_
#define _STAGESELECTCURSOR_H_

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
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
