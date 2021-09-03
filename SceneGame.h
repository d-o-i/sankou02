//=============================================================================
//
// ゲーム画面処理 [SceneGame.h]
// Author :  
//
//=============================================================================
#ifndef _SCENEGAME_H_
#define _SCENEGAME_H_

#include "Scene.h"

// ポインタとして取得する必要のあるクラス
#include "Map.h"
#include "_2dobj.h"
#include "Quadtree.h"
#include "Player.h"
#include "PaintGroup.h"
#include "PaintManager.h"
#include "EffectManager.h"
#include "Object3D.h"
#include "Timer.h"
#include "ParticleManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define START_FRAME (240)	// スタートするまでのタイマー


//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneGame :
	public Scene
{
private:
	std::vector<_2dobj*>		UIObject;	// UI
	std::vector <Object3D*>		object3d;	// 3Dオブジェクト
	Map				*pMap;					// マップ
	Player			*pPlayer[PLAYER_MAX];	// プレイヤー
	QUADTREE		*Quadtree = nullptr;	// 四分木
	PaintGroup		*paintGroup = nullptr;	// ペイントグループ
	EffectManager	*pEffectManager;		// エフェクト管理
	Timer			*pTimer;				// タイマー
	ParticleManager *particleManager;		// パーティクルマネージャ

	int				startframe;				// 開始カウントダウン
	bool			result;					// 終了フラグ
	static ResultData data[PLAYER_MAX];		// 結果
	static int TheLastPlayer;				// 最下位のプレイヤー

	void Start();
	void Collision();
	void CheckResult();
	void InsertResult(int pNo);
	void Debug();

public:
	SceneGame();
	~SceneGame();
	void Update(int SceneID);
	void Draw();
	static ResultData *GetResultData(int rank);
	static int GetTheLastPlayer(void) { return SceneGame::TheLastPlayer; };
};


#endif
