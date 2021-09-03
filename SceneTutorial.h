//=============================================================================
//
// チュートリアルシーン [SceneTutorial.h]
// Author :  
//
//=============================================================================
#ifndef _SCENETUTORIAL_H_
#define _SCENETUTORIAL_H_

#include "Scene.h"
#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneTutorial :
	public Scene
{
private:
	std::vector<_2dobj*> obj;

public:
	SceneTutorial();
	~SceneTutorial();

	void Update(int SceneID);
	void Draw();
};

#endif
