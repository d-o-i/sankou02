//=============================================================================
//
// メッシュ地面の処理 [MeshField.cpp]
// Author :  
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "Object3D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class MeshField :
	public Object3D
{
private:
	int NumBlockX, NumBlockZ;				// ブロック数
	int NumVertex;							// 総頂点数	
	int NumVertexIndex;						// 総インデックス数
	int NumPolygon;							// 総ポリゴン数
	float BlockSizeX, BlockSizeZ;			// ブロックサイズ

public:
	MeshField();
	~MeshField();

	void Update();
	void Draw();

};

#endif
