//=============================================================================
//
// リザルト画面処理 [SceneGame.h]
// Author :  
//
//=============================================================================
#ifndef _SCENERESULT_H_
#define _SCENERESULT_H_

#include "Scene.h"
#include "_2dobj.h"
#include "Timer.h"
#include "Object3D.h"
#include "ParticleManager.h"
#include "D3DXAnimation.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneResult :
	public Scene
{
private:
	std::vector<_2dobj*>		Obj2d;		// 2Dオブジェクト用のポインタ
	std::vector<D3DXANIMATION*>	anim;		// 3Dモデル用のポインタ（Xファイル）
	std::vector < Object3D*>	Obj3d;		// 3Dポリゴン用のポインタ
	ParticleManager*			particleManager;	// パーティクルマネージャ

	void Debug();

public:
	SceneResult();
	~SceneResult();

	void Update(int SceneID);
	void Draw();
};

#endif
