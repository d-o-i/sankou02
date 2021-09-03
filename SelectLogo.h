//=============================================================================
//
// キャラクターセレクトロゴ [SelectLogo.h]
// Author :  
//
//=============================================================================
#ifndef _SELECTLOGO_H_
#define _SELECTLOGO_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class SelectLogo :
	public _2dobj {
public:
	SelectLogo();
	~SelectLogo();

	// オーバーライド関数
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex();
};


#endif