//=============================================================================
//
// 四分木クラス[Quadtree.cpp]
// Author :  
//
//=============================================================================
#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Paint.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum e_QuadType
{
	ROOT,
	RightUpQuad,	// 右上象限(I)
	LeftUpQuad,		// 左上象限(II)
	LeftDownQuad,	// 左下象限(III)
	RightDownQuad,	// 右下象限(IV)
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class QUADTREE
{
private:
	std::vector<Paint*> Objects;	// 保存したオブジェクト
	QUADTREE *RightUpNode;			// 右上象限(I)
	QUADTREE *LeftUpNode;			// 左上象限(II)
	QUADTREE *LeftDownNode;			// 左下象限(III)
	QUADTREE *RightDownNode;		// 右下象限(IV)
	float x;						// ツリーの座標(左上が原点)
	float y;
	float Width;					// ツリーの横幅
	float Height;					// ツリーの高さ
	long NodeID;					// ノードの番号
	bool IsLeaf;					// このノードは葉っぱなのか
	int Level;						// 現在のレベル

   // 容量が足りない、新しいツリー(葉っぱ)を分割
	void CreateLeaves(void);
	// 子供ツリーを検査
	void CheckLeaves(void);
	// 子供ツリーを削除
	void DestroyLeaves(void);
	// 親のオブジェクトが子供ツリーに移動
	void MoveObjectsToLeaves(void);
	// 子供のオブジェクトが親ツリーに移動
	void MoveObjectsToNode(void);
	// オブジェクトはこのツリーの中を確認
	bool IsContain(Paint *Object) const;
	// 子供ツリーのノード番号を計算
	int CalChildNodeID(e_QuadType QuadType);
	// ノード番号で所属するツリーを検査
	int FindNode(int NodeID);
	// 全てのオブジェクトをクリア
	void Clear(void);
	// このノード以下の全オブジェクトを取得
	void GetNodeObject(std::vector<Paint*> *VectorPtr);
	// このノード以下の全オブジェクトの数を取得
	int GetNodeObjectNum(void);

public:
	// コンストラクタ
	QUADTREE();
	QUADTREE(float x, float y, float Width, float Height, int Level, int NodeID);
	// デストラクタ
	~QUADTREE();
	void Update();

	// オブジェクトを入れる
	void InsertObject(Paint *Object);
	// ツリーのオブジェクトを取得
	//std::vector<Paint*> GetObjectsAt(int NodeID, int Owner);
	std::vector<Paint*> GetObjectsAt(int NodeID);
	// オブジェクトがノードの範囲内かどうかを調べる
	bool CheckObjectInNode(Paint *Object);
};

#endif
