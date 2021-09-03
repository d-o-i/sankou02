//=============================================================================
//
// 紙吹雪 [Confetti.h]
// Author :  
//
//=============================================================================
#ifndef _CONFETTI_H_
#define _CONFETTI_H_

#include "Billboard.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Confetti :
	public Billboard
{
private:
	bool						use;			// 使用しているかどうか
	int							texNo;			// 使用するテクスチャ番号
	int							moveCnt;		// 動きをつけるタイミング
	static LPDIRECT3DTEXTURE9	D3DTexture;

	HRESULT MakeVertex();

public:
	Confetti();
	~Confetti();

	void Update();
	void Draw();

	bool GetUse() { return use; };

	static void ReleaseTexture() { SAFE_RELEASE(D3DTexture); };

};

#endif
