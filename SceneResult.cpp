//=============================================================================
//
// ゲーム画面処理 [SceneGame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneResult.h"
#include "Camera.h"
#include "Result.h"
#include "Input.h"
#include "SceneGame.h"
#include "Player.h"
#include "DebugWindow.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "ResultRank.h"
#include "ResultTimer.h"
#include "Podium.h"
#include "Trophy.h"
#include "ResultPlayer.h"
#include "SkyBox.h"
#include "MeshField.h"
#include "CircleSceneChanger.h"
#include "Confetti.h"

//=============================================================================
// コンストラクタ
//=============================================================================
SceneResult::SceneResult()
{
	// 2Dオブジェクトのインスタンスを作成
	Obj2d.push_back(new RESULT());
	// 1位から順に生成
	for (int resultNo = 0; resultNo < PLAYER_MAX; resultNo++)
	{
		Obj2d.push_back(new ResultRank(resultNo, SceneGame::GetResultData(resultNo)->playerNo));
		Obj2d.push_back(new ResultTimer(SceneGame::GetResultData(resultNo)->time, resultNo));
	}

	// 3Dモデルのインスタンス作成
	anim.push_back(new Podium());
	anim.push_back(new Trophy());
	// 1位から順に生成
	for (int resultNo = 0; resultNo < PLAYER_MAX; resultNo++)
	{
		anim.push_back(new ResultPlayer(resultNo, SceneGame::GetResultData(resultNo)->playerNo));
	}

	// 3Dポリゴンのインスタンス作成
	Obj3d.push_back(new SkyBox());
	Obj3d.push_back(new MeshField());

	// パーティクルマネージャのインスタンス作成
	particleManager = new ParticleManager();

/*****************************************************************************/
	// シーンチェンジの終了
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneResult::~SceneResult()
{
	// 2Dオブジェクトの削除
	for (auto &Obj : Obj2d)
	{
		SAFE_DELETE(Obj);
	}
	Obj2d.clear();
	ReleaseVector(Obj2d);

	// 3Dモデルの削除
	for (auto &Anim : anim)
	{
		SAFE_DELETE(Anim);
	}
	anim.clear();
	ReleaseVector(anim);

	// 3Dポリゴンの削除
	for (auto &Obj : Obj3d)
	{
		SAFE_DELETE(Obj);
	}
	Obj3d.clear();
	ReleaseVector(Obj3d);

	// パーティクルマネージャの削除
	delete particleManager;
}

//=============================================================================
// 更新
//=============================================================================
void SceneResult::Update(int SceneID)
{
	// カメラの更新
	UpdateCamera();

	// シーンチェンジ
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(i, BUTTON_C))
		{
			CircleSceneChanger::Instance()->SetChanger(true, [] 
			{
				SetScene(nSceneTitle);
				InitCamera();
			});
			return;
		}
	}

	// 2Dオブジェクトの更新
	for (auto &Obj : Obj2d)
	{
		Obj->Update();
	}

	// 3Dモデルの更新
	for (auto &Anim : anim)
	{
		Anim->Update();
	}

	// 3Dポリゴンの更新
	for (auto &Obj : Obj3d)
	{
		Obj->Update();
	}

	// 毎フレームパーティクルを発生させる
	particleManager->SetConfetti();

	// パーティクルマネージャの更新
	particleManager->Update();

#if _DEBUG
	//Debug();
#endif
}

//=============================================================================
// 描画
//=============================================================================
void SceneResult::Draw()
{
	// 3Dモデルの描画
	for (auto &Anim : anim)
	{
		Anim->Draw();
	}

	// 3Dポリゴンの描画
	for (auto &Obj : Obj3d)
	{
		Obj->Draw();
	}

	// パーティクルマネージャの描画
	particleManager->Draw();

	// 2Dオブジェクトの描画
	for (auto &Obj : Obj2d)
	{
		Obj->Draw();
	}
}

//=============================================================================
// デバッグ
//=============================================================================
void SceneResult::Debug()
{
#if _DEBUG_
	BeginDebugWindow("Result");

	DebugText("No1:%d No2:%d No3:%d No4:%d", SceneGame::GetResultData(0)->playerNo, SceneGame::GetResultData(1)->playerNo, SceneGame::GetResultData(2)->playerNo, SceneGame::GetResultData(3)->playerNo);
	DebugText("ResultTime\nNo1:%d No2:%d No3:%d No4:%d", SceneGame::GetResultData(0)->time, SceneGame::GetResultData(1)->time, SceneGame::GetResultData(2)->time, SceneGame::GetResultData(3)->time);

	EndDebugWindow("Result");

#endif
}