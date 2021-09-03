//=============================================================================
//
// ステージセレクト画面のロゴ [StageSelectLogo.h]
// Author :  
//
//=============================================================================
#ifndef _STAGESELECTLOGO_H_
#define _STAGESELECTLOGO_H_

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class StageSelectLogo :
	public _2dobj
{
private:
	int cntFlash;
	bool flash;		// 点滅させるフラグ(true = 表示)

	void MakeVertex();

public:
	StageSelectLogo();
	~StageSelectLogo();

	void Update()override;
	void Draw()override;

};

#endif
