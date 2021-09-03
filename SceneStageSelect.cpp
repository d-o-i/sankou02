//=============================================================================
//
// ステージセレクト [SceneStageSelect.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneStageSelect.h"
#include "SceneManager.h"
#include "SceneCharacterSelect.h"
#include "Input.h"
#include "CircleSceneChanger.h"

#include "StageSelectBG.h"
#include "StageName.h"
#include "StageSelectCursor.h"
#include "StageSelectLogo.h"

//=============================================================================
// コンストラクタ
//=============================================================================
SceneStageSelect::SceneStageSelect()
{
	// 背景
	obj.push_back(new StageSelectBG());
	// カーソル
	obj.push_back(new StageSelectCursor());
	// ステージ名
	obj.push_back(new StageName());
	// ロゴ
	obj.push_back(new StageSelectLogo());

/*****************************************************************************/
	// シーンチェンジの終了
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneStageSelect::~SceneStageSelect()
{
	// 2Dオブジェクトの削除
	for (auto &UI : obj)
	{
		SAFE_DELETE(UI);
	}
	obj.clear();
	ReleaseVector(obj);

}

//=============================================================================
// 更新
//=============================================================================
void SceneStageSelect::Update(int SceneID)
{
	// シーン遷移
	for (int playerNo = 0; playerNo < GAMEPAD_MAX; playerNo++)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(playerNo, BUTTON_C))
		{
			CircleSceneChanger::Instance()->SetChanger(true, []() 
			{
				SetScene(nSceneCharacterSelect);
			});
			return;
		}
	}

	// 2Dオブジェクトの更新
	for (auto & UI : obj)
	{
		UI->Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void SceneStageSelect::Draw()
{
	// 2Dオブジェクトの描画
	for (auto & UI : obj)
	{
		UI->Draw();
	}
}