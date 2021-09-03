//=============================================================================
//
// キャラクターセレクトフレーム [CharSelectFrame.h]
// Author :  
//
//=============================================================================
#ifndef _CharSelectFrame_H_
#define _CharSelectFrame_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
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

	// オーバーライド関数
	void Update();
	void Draw();
	void SetUse(bool Flag) { this->use = Flag; };
};


#endif