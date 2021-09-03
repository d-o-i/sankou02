//=============================================================================
//
// マップ処理 [Map.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Map.h"
#include "Input.h"
#include "MyLibrary.h"
#include "Collision.h"
#include "StageSelectBG.h"
#include "SceneManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// マップデータ
const char* MapFile[] = {
	("data/MAP/Gourmet_ground.csv"),
	("data/MAP/Ice_ground.csv"),
	("data/MAP/Lava_ground.csv"),
};

const char* ObjectFile[] = {
	("data/MAP/Gourmet_object.csv"),
	("data/MAP/Ice_object.csv"),
	("data/MAP/Lava_object.csv"),
};

//*****************************************************************************
// メンバ変数の初期化
//*****************************************************************************
std::vector<std::vector<int>> Map::maptbl;
std::vector<std::vector<int>> Map::objtbl;

//=============================================================================
// コンストラクタ
//=============================================================================
Map::Map()
{
	this->maptbl.reserve(MAP_SIZE_Y);
	for (int i = 0; i < MAP_SIZE_Y; i++)
	{
		vector<int> Vector_X;
		maptbl.push_back(Vector_X);
		maptbl.at(i).reserve(MAP_SIZE_X);
	}

	this->objtbl.reserve(MAP_SIZE_Y);
	for (int i = 0; i < MAP_SIZE_Y; i++)
	{
		vector<int> Vector_X;
		objtbl.push_back(Vector_X);
		objtbl.at(i).reserve(MAP_SIZE_X);
	}

	// csvデータ読み込み
	if (GetScene() == nSceneGame)
	{
		ReadCsv(MapFile[StageSelectBG::GetStageSelect()], &this->maptbl);
		ReadCsv(ObjectFile[StageSelectBG::GetStageSelect()], &this->objtbl);
	}
	else if (GetScene() == nSceneTitle)
	{
		int r = rand() % 3;

		ReadCsv(MapFile[r], &this->maptbl);
		ReadCsv(ObjectFile[r], &this->objtbl);
	}

	for (int cntY = 0; cntY < MAP_SIZE_Y; cntY++)
	{
		for (int cntX = 0; cntX < MAP_SIZE_X; cntX++)
		{
			if (maptbl.at(cntY).at(cntX) >= 0)
			{
				MapChipVector.push_back(new Chip(cntX, cntY, maptbl.at(cntY).at(cntX), eMapChip));
			}

			if (objtbl.at(cntY).at(cntX) >= 0)
			{
				ObjectChipVector.push_back(new Chip(cntX, cntY, objtbl.at(cntY).at(cntX), eObjectChip));
			}
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
Map::~Map()
{
	// マップテーブルクリア
	maptbl.clear();
	ReleaseVector(maptbl);

	// オブジェクトテーブルクリア
	objtbl.clear();
	ReleaseVector(objtbl);

	// マップチップベクトルクリア
	for (auto &MapChip : this->MapChipVector)
	{
		SAFE_DELETE(MapChip);
	}
	MapChipVector.clear();
	ReleaseVector(MapChipVector);

	// オブジェクトチップベクトルクリア
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->SetCnt(0);
		SAFE_DELETE(ObjectChip);
	}
	ObjectChipVector.clear();
	ReleaseVector(ObjectChipVector);
}

//=============================================================================
// 更新
//=============================================================================
void Map::Update()
{
	for (auto &MapChip : this->MapChipVector)
	{
		MapChip->Update();
	}
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void Map::Draw()
{
	for (auto &MapChip : this->MapChipVector)
	{
		MapChip->Draw();
	}
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->Draw();
	}
}

//=============================================================================
// ペイントマネージャとの当たり判定
//=============================================================================
void Map::PaintCollider(QUADTREE *Quadtree, int NodeID)
{
	std::vector<Paint*> CollisionList = Quadtree->GetObjectsAt(NodeID);

	// 現在のノードはオブジェクトがない
	if (CollisionList.empty())
	{
		return;
	}

	for (auto &BlackPaint : CollisionList)
	{
		// 使用している、カラーが黒のものを探す
		if (!BlackPaint->GetUse() || BlackPaint->GetPaintColor() != BlackInkColor)
			continue;

		for (auto &Obj : ObjectChipVector)
		{
			// 使用していないまたはすでに反転している場合は判定しない
			if (!Obj->GetUse() || Obj->GetReverse())
				continue;

			if (HitSphere(BlackPaint->GetPos(), Obj->GetPos(), Paint::GetPaintRadius(), CHIP_SIZE))
			{
				BlackPaint->SetUse(false);
				// フィールドオブジェクトを反転させる
				Obj->ReverseTexture();
				Obj->SetReverse(true);
				break;
			}
		}
	}
}

//=============================================================================
// 現在の座標から足元マップチップのXYを取得する
//=============================================================================
void Map::GetMapChipXY(D3DXVECTOR3 Pos, int *MapX, int *MapY)
{
	*MapX = (int)((Pos.x + CHIP_SIZE / 2) / CHIP_SIZE);
	*MapY = abs((int)((Pos.y - CHIP_SIZE / 2) / CHIP_SIZE));
}

//=============================================================================
// マップチップの座標を取得する
//=============================================================================
D3DXVECTOR3 Map::GetMapChipPos(int x, int y, int PosType)
{
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 2DのY座標と3DのY座標の方向は逆
	switch (PosType)
	{
	case eLeftUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE - CHIP_SIZE / 2, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	case eLeftCenter:
		Pos = D3DXVECTOR3(x * CHIP_SIZE - CHIP_SIZE / 2, -(y * CHIP_SIZE), 0.0f);
		break;
	case eRightUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE + CHIP_SIZE / 2, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	case eCenter:
		Pos = D3DXVECTOR3(x * CHIP_SIZE, -(y * CHIP_SIZE), 0.0f);
		break;
	case eCenterUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	default:
		break;
	}

	return Pos;
}

//=============================================================================
// マップチップXYからマップテーブルの数値を取得する
//=============================================================================
int Map::GetMapTbl(int MapX, int MapY)
{
	if (MapX < 0 ||
		MapX >= MAP_SIZE_X ||
		MapY < 0 ||
		MapY >= MAP_SIZE_Y)
	{
		return -1;
	}
	else
	{
		return maptbl.at(MapY).at(MapX);
	}
}

//=============================================================================
// 座標からマップテーブルの数値を取得する
//=============================================================================
// □□□
// □■□	■：CenterChip
// □□□
int Map::GetMapTbl(D3DXVECTOR3 Pos, int ChipDirection)
{
	int x = 0;
	int y = 0;

	GetMapChipXY(Pos, &x, &y);

	switch (ChipDirection)
	{
	case eCenterUp:
		// 中央の上のチップ
		y--;
		break;
	default:
		break;
	}

	return GetMapTbl(x, y);
}

//=============================================================================
// オブジェクトチップXYからオブジェクトテーブルの数値を取得する
//=============================================================================
int Map::GetObjTbl(int ObjX, int ObjY)
{
	if (ObjX < 0 ||
		ObjX >= MAP_SIZE_X ||
		ObjY < 0 ||
		ObjY >= MAP_SIZE_Y)
	{
		return -1;
	}
	else
	{
		return objtbl.at(ObjY).at(ObjX);
	}
}

//=============================================================================
// 座標からマップテーブルの数値を取得する
//=============================================================================
// □□□
// □■□	■：CenterChip
// □□□
int Map::GetObjTbl(D3DXVECTOR3 Pos, int ChipDirection)
{
	int x = 0;
	int y = 0;

	GetMapChipXY(Pos, &x, &y);

	switch (ChipDirection)
	{
	case eCenterUp:
		// 中央の上のチップ
		y--;
		break;
	default:
		break;
	}

	return GetObjTbl(x, y);
}

void Map::SetObjTbl(int ObjX, int ObjY, int texnum)
{
	objtbl.at(ObjY).at(ObjX) = texnum;
}