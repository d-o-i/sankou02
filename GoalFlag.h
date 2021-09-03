//=============================================================================
//
// ゴール地点表示 [GoalFlag.h]
// Author :  
//
//=============================================================================
#ifndef _GOALFLAG_H_
#define _GOALFLAG_H_

#include "Object3D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class GoalFlag :
	public Object3D
{
public:
	GoalFlag();
	~GoalFlag();

	void Update()override;
	void Draw()override;

	HRESULT MakeVertex();
};

#endif
