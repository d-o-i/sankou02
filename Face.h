//=============================================================================
//
// キャラクターの顔表示 [Face.h]
// Author :  
//
//=============================================================================
#ifndef _FACE_H_
#define _FACE_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Face :
	public _2dobj
{
private:
	int playerNo;	// 参照するプレイヤー番号
	int charNo;		// 選択したキャラクター番号

public:
	Face(int playerNo, int charNo);
	~Face();

	// オーバーライド関数
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture();
	void SetVertex();

};

#endif