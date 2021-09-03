//=============================================================================
//
// ペイント管理クラス [PaintSystem.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "PaintManager.h"
#include "Input.h"
#include "MyLibrary.h"
#include "Camera.h"
#include "Collision.h"
#include "Quadtree.h"
#include "DebugWindow.h"
#include "SceneGame.h"
#include "Player.h"
#include "MyLibrary.h"

#define HEAL_FRAME		(30)	// 何フレームごとに自動回復を行うか

QUADTREE *PaintManager::Quadtree = nullptr;
PaintGroup *PaintManager::paintGroup = nullptr;

//=============================================================================
// コンストラクタ
//=============================================================================
PaintManager::PaintManager(int PlayerNo, bool AIUse, CharacterAI *AIptr)
{
	this->Owner = PlayerNo;
	this->InkType = ColorInk;
	if (AIUse)
	{
		this->AIUse = true;
		this->AIptr = AIptr;
		// カーソルオブジェクト作成
		this->pCursor = new Cursor(this->Owner, true, AIptr);
		CharacterAI::SetPaintGroupPtr(PaintManager::paintGroup);
	}
	else
	{
		this->AIUse = false;
		this->AIptr = nullptr;
		// カーソルオブジェクト作成
		this->pCursor = new Cursor(this->Owner, false, nullptr);
	}

	// インク残量が最大値にする
	for (int i = 0; i < InkNum; i++)
	{
		this->InkValue[i] = INK_MAX;
	}

	// インクゲージUIオブジェクト作成
	this->inkGauge.push_back(new InkGauge(ColorInk, PlayerNo));
	this->inkGauge.push_back(new InkGauge(BlackInk, PlayerNo));

	this->SpInk = false;
	this->HealCnt = 0;

	// ペイントベクトルのメモリ領域確保
	BlackPaint.reserve(INK_MAX);
	ColorPaint.reserve(INK_MAX);
}

//=============================================================================
// デストラクタ
//=============================================================================
PaintManager::~PaintManager()
{
	// メモリリリース
	SAFE_DELETE(this->pCursor);



	for (auto &Object : this->inkGauge)
	{
		SAFE_DELETE(Object);
	}
	this->inkGauge.clear();
	ReleaseVector(inkGauge);

	for (auto &Paint : this->BlackPaint)
	{
		SAFE_DELETE(Paint);
	}
	this->BlackPaint.clear();
	ReleaseVector(BlackPaint);

	for (auto &Paint : this->ColorPaint)
	{
		SAFE_DELETE(Paint);
	}
	this->ColorPaint.clear();
	ReleaseVector(ColorPaint);
}

//=============================================================================
// 更新処理
//=============================================================================
void PaintManager::Update()
{
	// インクの自動回復
	AutoHeal();

	// カーソルの更新
	this->pCursor->Update();

	// 使用していないペイントがベクトルから削除
	CheckPaintUse();

	// 使用しているペイントを更新
	for (auto &Black : this->BlackPaint)
	{
		// オブジェクトが画面内かつ、ノードの範囲内じゃないなら、もう一度四分木に入れる
		if (Black->GetInScreen() && !PaintManager::Quadtree->CheckObjectInNode(Black))
		{
			PaintManager::Quadtree->InsertObject(Black);
		}
		Black->Update();
	}
	for (auto &Color : this->ColorPaint)
	{
		if (Color->GetInScreen() && !PaintManager::Quadtree->CheckObjectInNode(Color))
		{
			PaintManager::Quadtree->InsertObject(Color);
		}
		Color->Update();
	}

	// インクの種類交換
	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(Owner, BUTTON_R1) ||
		(AIUse && AIptr->GetAIChangeInk()))
	{
		InkType = InkType == BlackInk ? ColorInk : BlackInk;
		pCursor->ChangeInk();
		if (AIUse)
		{
			AIptr->SetInkType(InkType);
			AIptr->SetAIChangeInk(false);
		}
	}

	// インクを使う
	if (!AIUse)
	{
		// 使用するインクの残量チェック
		if (this->InkValue[InkType] > 0)
		{
			if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(Owner, BUTTON_C))
			{
				PaintManager::paintGroup->Start(Owner);
			}

			if (GetKeyboardPress(DIK_O) || IsButtonPressed(this->Owner, BUTTON_C))
			{
				// ペイントを設置する
				SetPaint(InkType);
				// インクを減らす
				if (!SpInk)
				{
					InkValue[InkType]--;
				}
			}

			if ((GetKeyboardRelease(DIK_O) || IsButtonReleased(Owner, BUTTON_C)) ||
				InkValue[InkType] <= 0)
			{
				PaintManager::paintGroup->End(Owner);
			}
		}
	}
	// AIがインクを使う
	else
	{
		if (AIptr->GetPaintState() == ePaintStart)
		{
			if (InkType == ColorInk)
			{
				PaintManager::paintGroup->Start(Owner);
			}
			AIptr->SetPaintState(ePainting);

			// ペイントを設置する
			SetPaint(InkType);
		}
		else if (AIptr->GetPaintState() == ePainting)
		{
			// 使用するインクの残量チェック
			if (this->InkValue[InkType] > 0)
			{
				// ペイントを設置する
				SetPaint(InkType);
				// インクを減らす
				if (!SpInk)
				{
					InkValue[InkType]--;

					// AIのインク残量状態を更新
					if (InkValue[InkType] >= INK_MAX)
					{
						// 満タン
						AIptr->SetInkState(InkType, eInkValue_Full);
					}
					else if (InkValue[InkType] < INK_MAX && InkValue[InkType] >= INK_MAX / 2)
					{
						// 半分以上
						AIptr->SetInkState(InkType, eInkValue_Many);
					}
					else if (InkValue[InkType] < INK_MAX / 2 && InkValue[InkType] >= INK_MAX * 0.15)
					{
						// 半分以下
						AIptr->SetInkState(InkType, eInkValue_Less);
					}
					else if (InkValue[InkType] < INK_MAX * 0.15)
					{
						// 15%以下
						AIptr->SetInkState(InkType, eInkValue_Few);
					}
				}
			}
			else
			{
				// インクの残量足りない、ペイント終了
				AIptr->SetPaintState(ePaintEnd);
			}
		}
		else if (AIptr->GetPaintState() == ePaintEnd)
		{
			// ペイントを設置する
			//SetPaint(InkType);

			// カラーインクなら、このペイントグループを記録する
			if (InkType == ColorInk)
			{
				PaintManager::paintGroup->End(Owner);
			}
			AIptr->SetPaintState(eNoAction);
		}
	}

	// インクゲージを更新
	inkGauge.at(ColorInk)->Update(InkValue[ColorInk]);
	inkGauge.at(BlackInk)->Update(InkValue[BlackInk]);

#if _DEBUG
	// インクの残量を調整
	if (GetKeyboardPress(DIK_Z))
	{
		InkValue[InkType] = max(--InkValue[InkType], 0);
	}

	if (GetKeyboardPress(DIK_X))
	{
		InkValue[InkType] = min(++InkValue[InkType], INK_MAX);
	}

	if (GetKeyboardPress(DIK_C))
	{
		InkValue[ColorInk] = INK_MAX;
		InkValue[BlackInk] = INK_MAX;
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		for (auto Paint = this->BlackPaint.begin(); Paint != this->BlackPaint.end(); Paint++)
		{
			(*Paint)->SetUse(false);
			SAFE_DELETE((*Paint));
		}
		this->BlackPaint.clear();

		for (auto Paint = this->ColorPaint.begin(); Paint != this->ColorPaint.end(); Paint++)
		{
			(*Paint)->SetUse(false);
			SAFE_DELETE((*Paint));
		}
		this->ColorPaint.clear();
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void PaintManager::Draw()
{
	// 使用しているペイントを描画
	for (auto &Black : this->BlackPaint)
	{
		Black->Draw();
	}
	for (auto &Color : this->ColorPaint)
	{
		Color->Draw();
	}

	// カーソルを描画
	this->pCursor->Draw();

	// インクゲージを描画
	// 現在使用しているインクはカラー、カラーインクゲージは前
	if (this->InkType == ColorInk)
	{
		inkGauge.at(BlackInk)->Draw();
		inkGauge.at(ColorInk)->Draw();
	}
	// 現在使用しているインクは黒、黒インクゲージは前
	else if (this->InkType == BlackInk)
	{
		inkGauge.at(ColorInk)->Draw();
		inkGauge.at(BlackInk)->Draw();
	}
}

//=============================================================================
// ペイントのセット
//=============================================================================
void PaintManager::SetPaint(int InkType)
{
	D3DXVECTOR3 CursorScreenPos = pCursor->GetPos();
	D3DXVECTOR3 CursorWorldPos = pCursor->GetWorldPos();

	// 黒インクの場合
	if (InkType == BlackInk)
	{
		if (this->BlackPaint.size() >= INK_MAX)
		{
			return;
		}

		Paint *Object = new Paint(this->Owner, BlackInkColor);

		Object->SetPos(CursorWorldPos);
		Object->SetUse(true);

		// スクリーン座標を保存する
		Object->SetScreenPos((D3DXVECTOR2)CursorScreenPos);
		// 四分木に入れる
		PaintManager::Quadtree->InsertObject(Object);

		// 使用しているインクの色に合わせて表示時間、テクスチャをセット
		Object->SetTime(DRAW_FRAME_BLACK);

		// Vectorに入れる
		this->BlackPaint.push_back(Object);
	}
	// カラーインクの場合
	else
	{
		if (this->ColorPaint.size() >= INK_MAX)
		{
			return;
		}

		Paint *Object = new Paint(this->Owner, this->Owner);

		Object->SetPos(CursorWorldPos);
		Object->SetUse(true);

		// スクリーン座標を保存する
		Object->SetScreenPos((D3DXVECTOR2)CursorScreenPos);
		// 四分木に入れる
		PaintManager::Quadtree->InsertObject(Object);

		PaintManager::paintGroup->Insert(CursorWorldPos, this->Owner);

		// 使用しているインクの色に合わせて表示時間、テクスチャをセット
		Object->SetTime(DRAW_FRAME_COLOR);

		// Vectorに入れる
		this->ColorPaint.push_back(Object);
	}
}

//=============================================================================
// 使用していないペイントがVectorから削除
//=============================================================================
void PaintManager::CheckPaintUse(void)
{
	for (auto Paint = this->BlackPaint.begin(); Paint != this->BlackPaint.end();)
	{
		if ((*Paint)->GetUse() == false)
		{
			SAFE_DELETE((*Paint));
			Paint = this->BlackPaint.erase(Paint);
		}
		else
		{
			Paint++;
		}
	}

	for (auto Paint = this->ColorPaint.begin(); Paint != this->ColorPaint.end();)
	{
		if ((*Paint)->GetUse() == false)
		{
			SAFE_DELETE((*Paint));
			Paint = this->ColorPaint.erase(Paint);
		}
		else
		{
			Paint++;
		}
	}
}

//=============================================================================
// 四分木から衝突可能なオブジェクトを探す
//=============================================================================
std::vector<Paint*> PaintManager::GetCollisionList(int NodeID)
{
	return PaintManager::Quadtree->GetObjectsAt(NodeID);
}

//=============================================================================
// 自動回復処理
//=============================================================================
void PaintManager::AutoHeal()
{
	HealCnt = LoopCountUp(HealCnt, 0, HEAL_FRAME);
	if (HealCnt == 0)
	{
		InkValue[ColorInk] = min(++InkValue[ColorInk], INK_MAX);
		InkValue[BlackInk] = min(++InkValue[BlackInk], INK_MAX);

		// AIのインク残量状態を更新
		if (AIUse)
		{
			for (int i = 0; i < InkNum; i++)
			{
				if (InkValue[i] >= INK_MAX)
				{
					// 満タン
					AIptr->SetInkState(i, eInkValue_Full);
				}
				else if (InkValue[i] < INK_MAX && InkValue[i] >= INK_MAX / 2)
				{
					// 半分以上
					AIptr->SetInkState(i, eInkValue_Many);
				}
				else if (InkValue[i] < INK_MAX / 2 && InkValue[i] >= INK_MAX * 0.15)
				{
					// 半分以下
					AIptr->SetInkState(i, eInkValue_Less);
				}
				else if (InkValue[i] < INK_MAX * 0.15)
				{
					// 15%以下
					AIptr->SetInkState(i, eInkValue_Few);
				}
			}
		}
	}
}