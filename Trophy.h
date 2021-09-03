//=============================================================================
//
// トロフィー[Trophy.h]
// Author :  
//
//=============================================================================
#ifndef _TROPHY_H_
#define _TROPHY_H_

#include "D3DXAnimation.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Trophy :
	public D3DXANIMATION
{
private:
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet();

public:
	Trophy();
	~Trophy();

	void Update()override;
	void Draw()override;
};

#endif
