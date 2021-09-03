//=============================================================================
//
// タイトル画面処理 [SceneTitle.h]
// Author : 
//
//=============================================================================
#ifndef _SCENETITLE_H_
#define _SCENETITLE_H_

#include "Scene.h"
class _2dobj;
class Map;
class Object3D;
class ParticleManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneTitle :
	public Scene
{
private:
	std::vector<_2dobj*> p2dObj;			// 2Dオブジェクト用のポインタ
	std::vector <Object3D*>	object3d;		// 3Dオブジェクト
	ParticleManager* particleManager;		// パーティクルマネージャ
	Map* map;								// マップ
	bool IsOption;							// 選択肢フラグ

public:
	SceneTitle();
	~SceneTitle();

	void Update(int SceneID);
	void Draw();
};

#endif
