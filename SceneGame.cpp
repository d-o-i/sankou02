//=============================================================================
//
// ゲーム画面処理 [SceneGame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Map.h"
#include "Camera.h"
#include "Collision.h"
#include "Input.h"
#include "DebugWindow.h"
#include "SceneResult.h"
#include "CircleSceneChanger.h"

// 2d obj
#include "Frame01.h"
#include "Face.h"
#include "CountDown.h"
#include "Item.h"
#include "Finish.h"

// 3d obj
#include "Sky.h"
#include "GoalFlag.h"

#include "Sound.h"
static int ResultRank[PLAYER_MAX];
//*****************************************************************************
// メンバ変数の初期化
//*****************************************************************************
ResultData SceneGame::data[PLAYER_MAX] = { NULL };		// 結果
int SceneGame::TheLastPlayer = 0;		// 結果

//=============================================================================
// コンストラクタ
//=============================================================================
SceneGame::SceneGame()
{
	// ゲームの結果を初期化
	startframe = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		data[i].playerNo = -1;
		data[i].time = 0;
	}
	result = false;

	// マップの初期化
	pMap = new Map();

	// 四分木の初期化
	Quadtree = new QUADTREE(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	PaintManager::SetQuadtreePtr(Quadtree);

	// ペイントグループの初期化
	paintGroup = new PaintGroup();
	PaintManager::SetPaintGroupPtr(paintGroup);

#if _DEBUG
	pPlayer[0] = new Player(0, true);
	pPlayer[1] = new Player(1, true);
	//pPlayer[1]->SetOnCamera(false);
	pPlayer[2] = new Player(2, true);
	//pPlayer[2]->SetOnCamera(false);
	pPlayer[3] = new Player(3, true);
	//pPlayer[3]->SetOnCamera(false);
#else
	// プレイヤーの初期化
	for (int PlayerNo = 0; PlayerNo < PLAYER_MAX; PlayerNo++)
	{
		pPlayer[PlayerNo] = new Player(PlayerNo, false);
	}
#endif

	// 2DUIの初期化
	// フレーム
	UIObject.push_back(new Frame());

	// アイテム表示の初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		UIObject.push_back(new Item(pPlayer[i]));
	}

	// カウントダウンの初期化
	UIObject.push_back(new CountDown());

	// エフェクトマネージャ
	pEffectManager = new EffectManager();

	// 3Dオブジェクト
	object3d.push_back(new Sky());
	object3d.push_back(new GoalFlag());

	// タイマー
	pTimer = new Timer();

	// パーティクルマネージャ
	particleManager = new ParticleManager();

	/*****************************************************************************/
		// シーンチェンジの終了
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneGame::~SceneGame()
{
	// マップの削除
	SAFE_DELETE(pMap);

	// 四分木の削除
	SAFE_DELETE(Quadtree);
	PaintManager::ReleaseQuadtreePtr();

	// ペイントグループの削除
	SAFE_DELETE(paintGroup);
	PaintManager::ReleasePaintGroupPtr();

	// プレイヤーの削除
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		SAFE_DELETE(pPlayer[i]);
	}

	// 2Dオブジェクトの削除
	for (auto &Object : UIObject)
	{
		SAFE_DELETE(Object);
	}
	UIObject.clear();
	ReleaseVector(UIObject);

	// エフェクトマネージャの削除
	SAFE_DELETE(pEffectManager);

	// 3Dオブジェクトの削除
	for (auto &Obj3D : object3d)
	{
		SAFE_DELETE(Obj3D);
	}
	object3d.clear();
	ReleaseVector(object3d);

	// タイマーの削除
	SAFE_DELETE(pTimer);

	// パーティクルマネージャの削除
	SAFE_DELETE(particleManager);
}

//=============================================================================
// 更新
//=============================================================================
void SceneGame::Update(int SceneID)
{
	// 開始処理
	if (startframe < START_FRAME)
	{
		Start();
	}

	float MaxPosX = 0.0f;
	float MinPosX = 100000.0f;
	int FirstPlayer = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			// プレイヤー座標の中でXが最も大きいものをカメラ注視点とする
			if (pPlayer[i]->GetPos().x > MaxPosX)
			{
				MaxPosX = pPlayer[i]->GetPos().x;
				FirstPlayer = i;
			}

			// プレイヤー座標の中でXが最も小さい
			if (pPlayer[i]->GetPos().x < MinPosX)
			{
				MinPosX = pPlayer[i]->GetPos().x;
				SceneGame::TheLastPlayer = i;
			}

			// 前方にプレイヤーがいるかどうかを確認
			if (pPlayer[i]->GetAIUse())
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 現在のプレイヤーとは違う
					if (i != j && pPlayer[j]->GetOnCamera())
					{
						// 現在のプレイヤーより右、かつ高さが同じぐらい
						if (pPlayer[i]->GetPos().x < pPlayer[j]->GetPos().x &&
							fabsf(pPlayer[i]->GetPos().y - pPlayer[j]->GetPos().y) < 10.0f)
						{
							pPlayer[i]->GetAIPtr()->SetShotBullet(true);
							break;
						}
						else
						{
							pPlayer[i]->GetAIPtr()->SetShotBullet(false);
						}
					}
				}
			}

		}
	}

	// カメラの更新
	UpdateCamera(pPlayer[FirstPlayer]->GetPos());

#if 0
	std::vector<float> vec(PLAYER_MAX);
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			vec.at(i) = pPlayer[i]->GetModel()->pos.x;
		}
	}
	auto max = std::max_element(vec.begin(), vec.end());
	size_t maxIdx = std::distance(vec.begin(), max);

	auto min = std::min_element(vec.begin(), vec.end());
	SceneGame::TheLastPlayer = (int)std::distance(vec.begin(), min);

	// カメラの更新
	UpdateCamera(pPlayer[(int)maxIdx]->GetModel()->pos);
#endif

	// マップの更新
	pMap->Update();

	// プレイヤーの更新
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		pPlayer[i]->Update();
	}

	// 当たり判定の更新
	Collision();

	// ペイントグループの更新
	paintGroup->Update();

	// 2Dオブジェクトの更新
	for (auto &Object : UIObject)
	{
		Object->Update();
	}

	// エフェクトマネージャの更新
	pEffectManager->Update();

	// 3Dオブジェクトの更新
	for (auto &Obj3D : object3d)
	{
		Obj3D->Update();
	}

	// タイマーの更新
	pTimer->Update();

	// パーティクルマネージャの更新
	particleManager->Update();

	// リザルト画面へ遷移していいか確認
	CheckResult();

	// デバッグ
	Debug();
}

//=============================================================================
// 描画
//=============================================================================
void SceneGame::Draw()
{
	// マップの描画
	pMap->Draw();

	// 3Dオブジェクトの描画
	for (auto &Obj3D : object3d)
	{
		Obj3D->Draw();
	}

	// プレイヤーの描画
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		pPlayer[i]->Draw();
	}

	// エフェクトマネージャの描画
	pEffectManager->Draw();

	// 2Dオブジェクトの描画
	for (auto &Object : UIObject)
	{
		Object->Draw();
	}

	// パーティクルマネージャの描画
	particleManager->Draw();

	// タイマーの描画
	pTimer->Draw();
}

//=============================================================================
// 当たり判定の更新
//=============================================================================
void SceneGame::Collision()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			// プレイヤーとマップの当たり判定
			pPlayer[i]->GroundCollider();
			pPlayer[i]->HorizonCollider();
			pPlayer[i]->ObjectCollider();
			pPlayer[i]->ObjectItemCollider(pMap);

			// プレイヤーとペイントマネージャの当たり判定
			pPlayer[i]->PaintCollider();
		}
	}

	// ペイントマネージャ同士の当たり判定
	for (int TenDigit = 1; TenDigit <= 4; TenDigit++)
	{
		for (int OneDigit = 1; OneDigit <= 4; OneDigit++)
		{
			std::vector<Paint*> CollisionList = Quadtree->GetObjectsAt((TenDigit * 10 + OneDigit));

			// 現在のノードはオブジェクトがない
			if (CollisionList.empty())
			{
				continue;
			}

			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (pPlayer[i]->GetOnCamera())
				{
					// 画面を16分割、それぞれのオブジェクトを判定する
					HitCheckSToS(&CollisionList, i);
				}
			}
		}
	}

	// フィールド上に発生したアイテムとの当たり判定
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nItem = 0; nItem < PLAYER_MAX; nItem++)
		{
			if (nPlayer != nItem)
			{
				pPlayer[nPlayer]->FieldItemCollider(pPlayer[nItem]->GetFieldItemManager());
			}
		}
	}

	// フィールドオブジェクトとペイントマネージャの当たり判定
	for (int TenDigit = 1; TenDigit <= 4; TenDigit++)
	{
		for (int OneDigit = 1; OneDigit <= 4; OneDigit++)
		{
			// 画面を16分割、それぞれのオブジェクトを判定する
			pMap->PaintCollider(Quadtree, (TenDigit * 10 + OneDigit));
		}
	}

	// 四分木を更新する
	Quadtree->Update();

}

//=============================================================================
// 開始処理
//=============================================================================
void SceneGame::Start()
{
	// スタートタイマー更新
	startframe++;

	if (startframe == START_FRAME)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			pPlayer[i]->SetPlayable(true);
		}

		pTimer->Start();
	}
}

//=============================================================================
// リザルト画面へ遷移していいか確認
//=============================================================================
void SceneGame::CheckResult()
{
#if _DEBUG
	if (GetKeyboardTrigger(DIK_N))
	{
		CircleSceneChanger::Instance()->SetChanger(true, []()
		{
			SetScene(nSceneResult);
			InitCamera();
		});
		return;
	}
#endif

	// 全員ゴールorゲームオーバーならシーン遷移可能
	if (result)
	{
		// タイマーストップ
		pTimer->Stop();

		for (int pNo = 0; pNo < PLAYER_MAX; pNo++)
		{
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(pNo, BUTTON_C))
			{
				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene(nSceneResult);
					InitCamera();
				});
				return;
			}
		}

		return;	// 全員ゴールしていたらここから先の処理はしない
	}

	// 全員がゴールorゲームオーバーになったか確認
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (data[i].playerNo != -1)
		{
			result = true;
		}
		else
		{
			result = false;
			break;
		}
	}

	// 全員ゴールした瞬間のみ
	if (result)
	{
		UIObject.push_back(new Finish());
	}

	for (int pNo = 0; pNo < PLAYER_MAX; pNo++)
	{
		bool hit = false;
		// すでにそのプレイヤーの結果がリザルト順位配列に登録されているか確認
		for (int rNo = 0; rNo < PLAYER_MAX; rNo++)
		{
			if (data[rNo].playerNo != pNo)
			{
				hit = false;
			}
			else
			{
				hit = true;
				break;
			}
		}

		if (!hit)
		{
			// まだ順位が登録されていない場合
			InsertResult(pNo);
		}
	}
}

//=============================================================================
// リザルト順位配列にデータの挿入
//=============================================================================
void SceneGame::InsertResult(int pNo)
{
	// ゲームオーバー確認
	if (!pPlayer[pNo]->GetOnCamera())
	{
		// リザルト順位配列の後ろから入れていく
		for (int rNo = PLAYER_MAX - 1; rNo > 0; rNo--)
		{
			if (data[rNo].playerNo == -1)
			{
				data[rNo].playerNo = pNo;
				data[rNo].time = 359999;
				break;
			}
		}
	}

	// ゴール確認
	if (pPlayer[pNo]->GetPos().x >= GOAL_POS.x)
	{
		// リザルト順位配列の前から入れていく
		for (int rNo = 0; rNo < PLAYER_MAX; rNo++)
		{
			if (data[rNo].playerNo == -1)
			{
				data[rNo].playerNo = pNo;
				data[rNo].time = pTimer->Check();
				break;
			}
		}
	}
}

//=============================================================================
// デバッグ
//=============================================================================
void SceneGame::Debug()
{
#if _DEBUG_
	BeginDebugWindow("Result");

	DebugText("All Goal or Gameover : %s", result ? "True" : "False");
	DebugText("No1:%d No2:%d No3:%d No4:%d", data[0].playerNo, data[1].playerNo, data[2].playerNo, data[3].playerNo);
	DebugText("ResultTime\nNo1:%d No2:%d No3:%d No4:%d", data[0].time, data[1].time, data[2].time, data[3].time);

	EndDebugWindow("Result");

#endif
}

//=============================================================================
// 結果のゲッター
//=============================================================================
ResultData *SceneGame::GetResultData(int rank)
{
	return &data[rank];
}
