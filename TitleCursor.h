//=============================================================================
//
// タイトル表示処理 [TitleCursor.h]
// Author :  
//
//=============================================================================
#ifndef _TITLECURSOR_H_
#define _TITLECURSOR_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************

class TITLECURSOR :
	public _2dobj
{
public:
	TITLECURSOR();
	~TITLECURSOR();

	// オーバーライド関数
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex(void);
	void SetVertexMove(D3DXVECTOR3 pos);

};

#endif 
