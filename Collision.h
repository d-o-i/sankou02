//=============================================================================
//
// コリジョン[Collision.h]
// Author : 
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Quadtree.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool HitCheckBB(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR2 sizeA, D3DXVECTOR2 sizeB);	// 矩形の当たり判定
bool HitSphere(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, float Range1, float Range2);				// 球の当たり判定

// ペイントシステム同士の当たり判定
//void HitCheckSToS(QUADTREE *Quadtree, int NodeID);
void HitCheckSToS(std::vector<Paint*> *CollisionList, int PlayerNo);

float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);											// 内積
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);						// 外積
int hitCheck(D3DXVECTOR3 *OutPos, TriangleStr tri, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		// ポリゴンと線分の当たり判定

#endif

