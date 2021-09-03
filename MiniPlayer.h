//=============================================================================
//
// ミニプレイヤー処理 [MiniPlayer.h]
// Author :  
//
//=============================================================================
#ifndef _MINIPLAYER_H_
#define _MINIPLAYER_H_

#include "_2dobj.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class MiniPlayer : public _2dobj
{
private:
	HRESULT MakeVertex(int PlayerNo);	// 頂点情報の作成
	void SetTexture(int PlayerNo);		// テクスチャ座標の設定
	void SetVertex(void);				// 頂点座標の設定

public:
	MiniPlayer(int PlayerNo);
	~MiniPlayer();

	void Update(D3DXVECTOR3 PlayerPos);
	void Draw();
};

#endif

