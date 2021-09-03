//=============================================================================
//
// マップ処理 [Map.h]
// Author :  
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "Chip.h"
#include "Quadtree.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define MAP_SIZE_X		(500)								// マップの横の枚数
#define MAP_SIZE_Y		(50)								// マップの縦の枚数
#define START_POS		D3DXVECTOR3(50.0f, 0.0f, 0.0f)		// スタート地点
//#define GOAL_POS		D3DXVECTOR3(500.0f, 0.0f, 0.0f)	// ゴール地点
#define GOAL_POS		D3DXVECTOR3(9800.0f, 0.0f, 0.0f)		// ゴール地点

// マップチップ座標を取得の種類
enum e_ChipPosType
{
	eLeftUp,	// マップチップの左上の座標 
	eLeftCenter,// マップチップの左中央の座標 
	eRightUp,	// マップチップの右上の座標 
	eCenter,	// マップチップの中心の座標 
	eCenterUp,	// マップチップの中央の上の座標
};

// フィールドオブジェクトの種類
enum e_ChipType
{
	eObjSpdup,		// スピードアップ
	eObjSpddown,	// スピードダウン
	eObjNuma,		// 沼（スピードダウン＆ジャンプ力ダウン）
	eObjJump,		// 強制ジャンプ
	eObjDrain,		// インクゲージ減少
	eObjHeal,		// インクゲージ増加
	eObjItem,		// アイテム取得
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Map
{
private:
	static std::vector<std::vector<int>>	maptbl;
	std::vector<Chip*>						MapChipVector;

	static std::vector<std::vector<int>>	objtbl;
	std::vector<Chip*>						ObjectChipVector;

public:
	Map();
	~Map();

	void Update();
	void Draw();

	void PaintCollider(QUADTREE *Quadtree, int NodeID);

	static int GetMapTbl(int MapX, int MapY);
	static int GetMapTbl(D3DXVECTOR3 Pos, int ChipDirection);
	static void GetMapChipXY(D3DXVECTOR3 Pos, int *MapX, int *MapY);
	static D3DXVECTOR3 GetMapChipPos(int x, int y, int PosType);
	static int GetObjTbl(int ObjX, int ObjY);
	static int GetObjTbl(D3DXVECTOR3 Pos, int ChipDirection);

	static void SetObjTbl(int ObjX, int ObjY, int texnum);
	std::vector<Chip*> GetObjectChip() { return ObjectChipVector; };

};

#endif
