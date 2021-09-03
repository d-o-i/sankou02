//=============================================================================
//
// タイトル画面処理 [SceneTitle.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneTitle.h"
#include "_2dobj.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "Sound.h"
#include "SceneCharacterSelect.h"
#include "TitleCursor.h"
#include "SceneTutorial.h"
#include "Player.h"
#include "CircleSceneChanger.h"
#include "SceneExit.h"
#include "Sky.h"
#include "Camera.h"
#include "Runner.h"
#include "ParticleManager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
SceneTitle::SceneTitle()
{
	
	IsOption = true;							// 選択肢フラグオン

	// UIオブジェクト
	p2dObj.push_back(new TITLE(TitleLogo));
	p2dObj.push_back(new TITLE(TitleMenu));
	p2dObj.push_back(new Runner());
	p2dObj.push_back(new TITLECURSOR());

	// マップ
	map = new Map();

	// 3Dオブジェクト
	object3d.push_back(new Sky());

	// パーティクルマネージャ
	particleManager = new ParticleManager();

/*****************************************************************************/
	// シーンチェンジの終了
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneTitle::~SceneTitle()
{
	// 2Dオブジェクトの開放
	for (auto &UI : p2dObj)
	{
		SAFE_DELETE(UI);
	}
	p2dObj.clear();
	ReleaseVector(p2dObj);

	// マップの開放
	SAFE_DELETE(map);

	// 3Dオブジェクトの開放
	for (auto &obj : object3d)
	{
		SAFE_DELETE(obj);
	}
	object3d.clear();
	ReleaseVector(object3d);

	// パーティクルマネージャの開放
	SAFE_DELETE(particleManager);
}

//=============================================================================
// 更新
//=============================================================================
void SceneTitle::Update(int SceneID)
{
	// モード選択
	for (int playerNo = 0; playerNo < PLAYER_MAX; playerNo++)
	{
		if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(playerNo, STICK_UP))
		{
			IsOption = true;
			break;
		}
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(playerNo, STICK_DOWN))
		{
			IsOption = false;
			break;
		}

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(playerNo, BUTTON_C))
		{
			if (IsOption == true)
			{
				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene(nSceneTutorial);
					InitCamera();
				});
				return;
			}
			else
			{
				//SetScene(new SceneExit(), nSceneExit);

				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene( nSceneExit);
				});
				return;
			}
		}


	}

	// カメラの更新
	UpdateTitleCamera();

	// 2Dオブジェクトの更新
	for (auto &UI : p2dObj)
	{
		UI->Update();
	}

	// マップの更新
	map->Update();

	// 3Dオブジェクトの更新
	for (auto &obj : object3d)
	{
		obj->Update();
	}

	// パーティクルマネージャの更新
	particleManager->Update();
}

//=============================================================================
// 描画
//=============================================================================
void SceneTitle::Draw()
{
	// マップの描画
	map->Draw();

	// 3Dオブジェクトの描画
	for (auto &obj : object3d)
	{
		obj->Draw();
	}

	// 2Dオブジェクトの描画
	for (auto &UI : p2dObj)
	{
		UI->Draw();
	}

	// パーティクルマネージャの描画
	particleManager->Draw();
}