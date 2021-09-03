//=============================================================================
//
// ステージセレクト [SceneStageSelect.h]
// Author :  
//
//=============================================================================
#ifndef _SCENESTAGESELECT_H_
#define _SCENESTAGESELECT_H_

#include "Scene.h"

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneStageSelect :
	public Scene
{
private:
	std::vector<_2dobj*> obj;

public:
	SceneStageSelect();
	~SceneStageSelect();

	void Update(int SceneID)override;
	void Draw()override;
};

#endif
