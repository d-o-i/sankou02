//=============================================================================
//
// コリジョン[Collision.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Collision.h"
#include "Input.h"
#include "Player.h"

//=============================================================================
// 矩形の当たり判定
//=============================================================================
bool HitCheckBB(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR2 sizeA, D3DXVECTOR2 sizeB)
{
	sizeA.x /= 2.0f;
	sizeA.y /= 2.0f;
	sizeB.x /= 2.0f;
	sizeB.y /= 2.0f;

	if ((posB.x + sizeB.x > posA.x - sizeA.x) && (posA.x + sizeA.x > posB.x - sizeB.x) &&
		(posB.y + sizeB.y > posA.y - sizeA.y) && (posA.y + sizeA.y > posB.y - sizeB.y))
	{
		return true;
	}

	return false;
}

//=============================================================================
// 球の当たり判定
//=============================================================================
bool HitSphere(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, float Radius1, float Radius2)
{
	// 当たり判定の中心の距離を測る
	float dist = D3DXVec3LengthSq(&D3DXVECTOR3(Pos1 - Pos2));

	// 当たり判定の範囲を足した距離を出す（球なのでXYZどれとっても同じ）
	float hitrange = powf((Radius1 + Radius2), 2);

	// 当たり判定の中心の距離よりも範囲を足した距離の方が長ければ当たる
	return dist <= hitrange ? true : false;
}

//=============================================================================
// ペイントシステム同士の当たり判定
//=============================================================================
void HitCheckSToS(std::vector<Paint*> *CollisionList, int PlayerNo)
{
	for (auto &BlackPaint : (*CollisionList))
	{
		// 使用している、色が黒、所有者が現在のプレイヤー
		if (!BlackPaint->GetUse() || BlackPaint->GetPaintColor() != BlackInkColor ||
			BlackPaint->GetOwner() != PlayerNo)
		{
			continue;
		}

		for (auto &ColorPaint : (*CollisionList))
		{
			// 使用している、色がカラー、所有者が現在のプレイヤーじゃない
			if (!ColorPaint->GetUse() || ColorPaint->GetPaintColor() == BlackInkColor ||
				ColorPaint->GetOwner() == PlayerNo)
			{
				continue;
			}

			// 二つのペイントを判定する
			if (HitSphere(BlackPaint->GetPos(), ColorPaint->GetPos(), Paint::GetPaintRadius(), Paint::GetPaintRadius()))
			{
				// ヒットした場合そのペイントを消す
				BlackPaint->SetUse(false);
				ColorPaint->SetUse(false);
				break;
			}
		}
	}
}

//=============================================================================
// 内積
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
#if 1
	float ans = vl->x * vr->x + vl->y * vr->y + vl->z * vr->z;
#else
	float ans = D3DXVec3Dot(vl, vr);
#endif	
	return ans;
}


//=============================================================================
// 外積
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
#if 1
	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
#else
	D3DXVec3Cross(ret, vl, vr);
#endif
}

//=============================================================================
// ポリゴンと線分との当たり判定
// OutPos :出力
// tri    :三角形ポリゴン
// pos0   :始点（移動前）
// pos1   :終点（移動後）
//=============================================================================
int hitCheck(D3DXVECTOR3 *OutPos, TriangleStr tri, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// ポリゴンの法線

	// ポリゴンの外積をとって法線を求める。そして正規化しておく。
	D3DXVECTOR3		vec01 = tri.pos1 - tri.pos0;
	D3DXVECTOR3		vec02 = tri.pos2 - tri.pos0;
	crossProduct(&nor, &vec01, &vec02);

	{
		// 平面と線分の内積とって衝突している可能性を調べる
		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		D3DXVECTOR3		vec1 = pos0 - tri.pos0;
		D3DXVECTOR3		vec2 = pos1 - tri.pos0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);

		if ((a * b) > 0)
		{
			// 当たっている可能性は無い
			return(0);
		}
	}


	{
		// ポリゴンと線分の交点を求める
		D3DXVECTOR3		vec1 = pos0 - tri.pos0;
		D3DXVECTOR3		vec2 = pos1 - tri.pos0;
		float			d1 = (float)fabs(dotProduct(&nor, &vec1));			// 内分比を求める為の点とポリゴンとの距離
		float			d2 = (float)fabs(dotProduct(&nor, &vec2));			// 内分比を求める為の点とポリゴンとの距離
		float			a = d1 / (d1 + d2);									// 内分比
		D3DXVECTOR3		vec3 = (1 - a) * vec1 + a * vec2;
		D3DXVECTOR3		p3 = tri.pos0 + vec3;										// 交点

		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			D3DXVECTOR3		v1 = tri.pos1 - tri.pos0;
			D3DXVECTOR3		v2 = tri.pos2 - tri.pos1;
			D3DXVECTOR3		v3 = p3 - tri.pos2;

			// 各頂点と交点とのベクトル
			D3DXVECTOR3		v4 = p3 - tri.pos1;
			D3DXVECTOR3		v5 = p3 - tri.pos2;
			D3DXVECTOR3		v6 = p3 - tri.pos0;

			// 各辺との外積で法線を求める
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);

			// それぞれのベクトルの向きを内積でチェック
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);
		}

		// 出力用の座標をセット
		*OutPos = p3;
	}

	return(1);
}