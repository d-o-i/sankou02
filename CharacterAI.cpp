//=============================================================================
//
// CharacterAIクラス[CharacterAI.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "CharacterAI.h"
#include "Map.h"
#include "Item.h"
#include "SceneGame.h"
#include "Input.h"
#include "DebugWindow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
PaintGroup *CharacterAI::paintGroup = nullptr;

#define ScanRange_Plat (10)		// プラットフォームのへりを探せる範囲
#define ScanRange_ObjChip (3)	// オブジェクトチップを探せる範囲

// CSV中の番号の事件
enum e_ChipEvent
{
	eNoEventChip = -1,
	eRandomChip = -2,		// ペイント、落下のどちらかランダムで決める
	eJumpChip = -3,			// ジャンプする
	ePaintChip = -4,		// ペイントする
	ePlatformEdge = -5,		// 向こうのプラットフォームのへり
	ePlatformEdge_Item = -6,// プラットフォームのへり(アイテムあり)
	ePaintObject = -7,		// 前方のオブジェクトチップをペイントする
};

#if _DEBUG
// 3D直線頂点フォーマット( 頂点座標[3D] / 反射光 )
#define	FVF_LINE_3D		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

// 3D空間で直線描画用構造体を定義
typedef struct
{
	D3DXVECTOR3 Point;
	D3DCOLOR	Color;
} VERTEX_3DLINE;
#endif

//=====================================================================================================
// コンストラクタ
//=====================================================================================================
CharacterAI::CharacterAI(int Owner)
{
	this->Owner = Owner;
	this->InkState[ColorInk] = eInkValue_Full;
	this->InkState[BlackInk] = eInkValue_Full;
}

//=====================================================================================================
// デストラクタ
//=====================================================================================================
CharacterAI::~CharacterAI()
{
	CharacterAI::paintGroup = nullptr;
}

//=====================================================================================================
// 更新処理
//=====================================================================================================
void CharacterAI::Update(D3DXVECTOR3 Pos)
{
	int MapChipNo = Map::GetMapTbl(Pos, eCenterUp);
	this->Action = eNoAction;
#if _DEBUG
	this->DrawLineFlag = false;
#endif

	// 暫くプレイヤーが動けないなら
	if (Pos == PrePos)
	{
		StopCount++;
		if (StopCount >= 30)
		{
			// ジャンプする
			this->Action = eActJump;
			StopCount = 0;
		}
	}
	else
	{
		StopCount = 0;
	}


	// マップチップの番号によって行動する
	MapChipAction(Pos, MapChipNo);

	// 画面内の他人のペイントを探して、削除する
	if (CursorState != ePaintPath)
	{
		PaintAction();
	}

	// アイテムを持っていたら、アイテムを使用する
	if (HaveItem)
	{
		ItemAction();
	}

	// 1フレーム前の座標を保存する
	PrePos = Pos;

#if _DEBUG
	ImGui::SetNextWindowPos(ImVec2(5, 330), ImGuiSetCond_Once);

	BeginDebugWindow("AI");

	DebugText("Action : %d", this->Action);
	DebugText("State : %d", this->CursorState);

	EndDebugWindow("AI");
#endif
}

//=====================================================================================================
// マップチップの番号によって行動する
//=====================================================================================================
void CharacterAI::MapChipAction(D3DXVECTOR3 Pos, int MapChipNo)
{
	// フラグの初期化
	if (MapChipNo == eNoEventChip)
	{
		RandomOver = false;
		FindEdgeOver = false;
		FindObjChipOver = false;
	}

	if (MapChipNo == eRandomChip && !RandomOver)
	{
		int Random = rand() % (2 + 1);

		// 落下
		if (Random == 0)
		{
			;	// 何もしない
		}
		// ペイント
		else
		{
			// プラットフォームのへりを探す
			this->FindPlatform(Pos);
		}

		RandomOver = true;
	}
	else if (MapChipNo == eJumpChip)
	{
		// ジャンプする
		this->Action = eActJump;
	}
	else if (MapChipNo == ePaintChip)
	{
		// プラットフォームのへりを探す
		this->FindPlatform(Pos);
	}
	else if (MapChipNo == ePaintObject && CursorState != eUseBlackPaint)
	{
		// ペイントするオブジェクトチップを探す
		this->FindObjectChip(Pos);
	}
}

//=====================================================================================================
// 他のプレイヤーのペイントを探して、削除する
//=====================================================================================================
void CharacterAI::PaintAction(void)
{
	bool Trigger = false;
	static int Count = 0;
	static int Bonus = 0;

	if (!FindEnemyPaint)
	{
		if (!SpecialInk)
		{
			Count++;
		}
		else
		{
			// スペシャルインクを使用中するなら、判定速度が速くなる
			Count += 2;
		}

		// 2秒ごとに判定する
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 6)
			{
				Trigger = true;
				Bonus = 0;
				Count = 0;
			}
			else
			{
				// 使用しないが、時間によって使用する確率が高くなる
				Bonus++;
			}
		}

		if (Trigger)
		{
			// 画面中のペイントを取得する
			if (CharacterAI::paintGroup->GetEnemyPaint(&EnemyPaint, Owner))
			{
				CursorState = eUseBlackPaint;
				if (InkType != BlackInk)
				{
					ChangeInk = true;
					InkType = BlackInk;
				}
				FindEnemyPaint = true;
			}
		}
	}
}

//=====================================================================================================
// アイテムを使用する
//=====================================================================================================
void CharacterAI::ItemAction(void)
{
	bool ItemTrigger = false;
	static int Count = 0;
	static int Bonus = 0;

	switch (ItemType)
	{
		// ジェットパック
	case NumJet:
		// パワーアップバナナ
	case NumPowerUp:
		// スペシャルインク
	case NumSpInk:
		// ブラインド
	case NumBlind:

		Count++;
		// 2秒ごとに判定する
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 5)
			{
				// アイテムを使用する
				ItemTrigger = true;
			}
			else
			{
				// 使用しないが、時間によって使用する確率が高くなる
				Bonus++;
			}
		}
		break;

		// スパイクブーツ
	case NumSpike:

		Count++;
		// 2秒ごとに判定する
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 5)
			{
				// アイテムを使用する
				ItemTrigger = true;
			}
			else
			{
				// 使用しないが、時間によって使用する確率が高くなる
				Bonus++;
			}
		}

		if (UseSpike)
		{
			// アイテムを使用する
			ItemTrigger = true;
		}
		break;

		// バナナの皮
	case NumKawa:

		// 最下位じゃなくて、後ろに他のプレイヤーがいるならバナナの皮を使う
		if (SceneGame::GetTheLastPlayer() != Owner)
		{
			// アイテムを使用する
			ItemTrigger = true;
		}
		else
		{
			Count++;
			// 2秒ごとに判定する
			if (Count % 120 == 0)
			{
				int Rand = rand() % (10 + 1);
				if (Rand + Bonus > 5)
				{
					// アイテムを使用する
					ItemTrigger = true;
				}
				else
				{
					// 使用しないが、時間によって使用する確率が高くなる
					Bonus++;
				}
			}
		}

		break;

		// トリモチガン
	case NumGun:

		Count++;
		// 0.5秒ごとに判定する
		if (Count % 30 == 0)
		{
			if (ShotBullet || Bonus >= 10)
			{
				// アイテムを使用する
				ItemTrigger = true;
				ShotBullet = false;
			}
			else
			{
				// 使用しないが、時間によって使用する確率が高くなる
				Bonus++;
			}
		}
		break;

	default:
		break;
	}

	// アイテムを使用する
	if (ItemTrigger)
	{
		HaveItem = false;
		UseItem = true;
		Count = 0;
		Bonus = 0;

		// スペシャルインクを使用中
		if (ItemType == NumSpInk)
		{
			SpecialInk = true;
		}
	}
}

//=====================================================================================================
// 向こうのプラットフォームを探す
//=====================================================================================================
void CharacterAI::FindPlatform(D3DXVECTOR3 Pos)
{
	int PlayerChip_X = 0;
	int PlayerChip_Y = 0;
	Map::GetMapChipXY(Pos, &PlayerChip_X, &PlayerChip_Y);
	std::vector<D3DXVECTOR3> TempPos;
	bool Trigger = false;

	if (FindObjChipOver || (InkState[ColorInk] == eInkValue_Few && !SpecialInk))
	{
		return;
	}

	// スキャンできる横幅の範囲
	for (int i = PlayerChip_X + 1; i <= PlayerChip_X + ScanRange_Plat; i++)
	{
		// 縦幅の範囲
		for (int j = 0; j < MAP_SIZE_Y; j++)
		{
			int k = Map::GetMapTbl(i, j);
			if (k == ePlatformEdge || k == ePlatformEdge_Item)
			{
				// 探したプラットフォームの座標
				TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));

				// 向こうにアイテムがあったら、行く確率が高くなる
				if (k == ePlatformEdge_Item)
				{
					TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));
					TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));
				}

				Trigger = true;
			}
		}
	}

	if (Trigger)
	{
		// キャラクター下のチップの座標(ペイントの始点)
		PaintStartPos = Map::GetMapChipPos(PlayerChip_X + 1, PlayerChip_Y, eRightUp);
		// カーソル状態を変わる
		CursorState = ePaintPath;
		// インク交換（必要があるなら）
		if (InkType != ColorInk)
		{
			ChangeInk = true;
			InkType = ColorInk;
		}
		FindEdgeOver = true;

#if _DEBUG
		DrawLineFlag = true;
#endif

		// もし複数のプラットフォームがあれば、ランダムで決める
		if (TempPos.size() > 1)
		{
			int Random = rand() % TempPos.size();
			PaintEndPos = TempPos.at(Random);
		}
		else if (TempPos.size() == 1)
		{
			PaintEndPos = TempPos.at(0);
		}
	}
}

//=====================================================================================================
// ペイントするオブジェクトチップを探す
//=====================================================================================================
void CharacterAI::FindObjectChip(D3DXVECTOR3 PlayerPos)
{
	int Chip_X = 0;
	int Chip_Y = 0;
	// プレイヤーの座標によって、チップのXY座標を取得
	Map::GetMapChipXY(PlayerPos, &Chip_X, &Chip_Y);
	bool FindStartChip = false;
	int ChipType = 0;

	if (FindObjChipOver || (InkState[BlackInk] == eInkValue_Few && !SpecialInk) || UseSpike)
	{
		return;
	}

	// 最初のオブジェクトチップの座標を探す
	for (int i = 0; i < ScanRange_ObjChip; i++)
	{
		int TempChip_X = Chip_X + 1 + i;
		for (int j = 0; j < ScanRange_ObjChip; j++)
		{
			int TempChip_Y = Chip_Y - (ScanRange_ObjChip / 2) + j;
			// オブジェクトチップテーブルから、チップ種類を取得
			ChipType = Map::GetObjTbl(TempChip_X, TempChip_Y);
			if (ChipType == eObjSpddown || ChipType == eObjNuma || ChipType == eObjDrain)
			{
				// スパイクブーツを持っているなら、先に使う
				if (HaveItem && ItemType == NumSpike)
				{
					UseSpike = true;
					return;
				}
				else
				{
					// 最初のオブジェクトチップの座標
					PaintStartPos = Map::GetMapChipPos(TempChip_X, TempChip_Y, eCenter);
					// ペイントの終了座標を保存
					PaintEndPos = Map::GetMapChipPos(TempChip_X, TempChip_Y, eCenter);
					// 最初のオブジェクトチップを探した
					FindStartChip = true;
				}
				break;
			}
		}

		if (FindStartChip)
		{
			break;
		}
	}

	// ペイントするオブジェクトチップの終点を探す
	if (FindStartChip)
	{
		//  □
		// ■□			■：PaintStartPos(起点)
		//  □			□：オブジェクトチップを探す範囲
		Map::GetMapChipXY(PaintStartPos, &Chip_X, &Chip_Y);
		Chip_X++;

		while (true)
		{
			int Count = 0;

			for (int j = 0; j < 3; j++)
			{
				int TempChip_Y = Chip_Y - 1 + j;
				// オブジェクトチップテーブルから、チップ種類を取得
				int TempChipNo = Map::GetObjTbl(Chip_X, TempChip_Y);
				// 探したチップと最初のチップは同じ種類
				if (ChipType == TempChipNo)
				{
					// ペイントの終点座標を保存
					PaintEndPos = Map::GetMapChipPos(Chip_X, TempChip_Y, eCenter);
					Chip_X++;
					Chip_Y = TempChip_Y;
					break;
				}
				else
				{
					Count++;
				}
			}

			// ループ終了
			if (Count >= 3)
			{
				CursorState = ePaintObjChip;
				// インク交換（必要があるなら）
				if (InkType != BlackInk)
				{
					ChangeInk = true;
					InkType = BlackInk;
				}
				break;
			}
		}
	}
}

#if _DEBUG
//=============================================================================
// 描画処理
//=============================================================================
void CharacterAI::Draw(void)
{
	if (this->DrawLineFlag)
	{
		//DrawLine3D(this->PaintStartPos, this->PaintEndPos);
	}
}

//=============================================================================
// 3D直線描画
//=============================================================================
void CharacterAI::DrawLine3D(D3DXVECTOR3 P1, D3DXVECTOR3 P2)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX WorldMatrix;
	LPDIRECT3DVERTEXBUFFER9 LineVtxBuffer;		// 直線描画用頂点バッファ
	VERTEX_3DLINE *pVtx;

	// 直線描画用頂点バッファを生成
	Device->CreateVertexBuffer(
		sizeof(VERTEX_3DLINE) * 2,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
		FVF_LINE_3D,					// 使用する頂点フォーマット
		D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
		&LineVtxBuffer,					// 頂点バッファインターフェースへのポインタ
		NULL);							// NULLに設定

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	LineVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].Point = P1;
	pVtx[1].Point = P2;

	// 色の設定
	pVtx[0].Color = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[1].Color = D3DCOLOR_RGBA(0, 0, 0, 255);

	// 頂点データをアンロックする
	LineVtxBuffer->Unlock();

	// ラインティングを無効にする
	Device->SetRenderState(D3DRS_LIGHTING, false);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMatrix);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// 頂点バッファをレンダリングパイプラインに設定
	Device->SetStreamSource(0, LineVtxBuffer, 0, sizeof(VERTEX_3DLINE));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_LINE_3D);

	// テクスチャの設定
	Device->SetTexture(0, NULL);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ラインティングを有効にする
	Device->SetRenderState(D3DRS_LIGHTING, true);

	SAFE_RELEASE(LineVtxBuffer);

	return;
}

#endif
