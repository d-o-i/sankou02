//=============================================================================
//
// ゲーム終了処理 [SceneExit.h]
// Author :  
//
//=============================================================================
#ifndef _SCENEEXIT_H_
#define _SCENEEXIT_H_

#include "Scene.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneExit :
	public Scene
{
public:
	SceneExit();
	~SceneExit();

	void Update(int SceneID)override;
	void Draw()override;
};

#endif
