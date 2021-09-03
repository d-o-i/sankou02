//=============================================================================
//
// リザルト画面でのプレイヤー [ResultPlayer.cpp]
// Author :  
//
//=============================================================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "D3DXAnimation.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class ResultPlayer :
	public D3DXANIMATION
{
private:
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet();

public:
	ResultPlayer(int rank, int owner);
	~ResultPlayer();

	void Update()override;
	void Draw()override;
};

#endif
