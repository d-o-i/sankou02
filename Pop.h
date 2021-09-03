//=============================================================================
//
// ポップアップ表示処理 [Pop.h]
// Author :  
//
//=============================================================================
#ifndef _POP_H_
#define _POP_H_

#include "Billboard.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Pop : public Billboard
{
private:
	int PlayerNo;

	HRESULT MakeVertex();

public:
	Pop(int PlayerNo);
	~Pop();

	void Update() {};
	void Update(D3DXVECTOR3 PlayerPos);
	void Draw();
};

#endif
