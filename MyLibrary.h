//=============================================================================
//
// その他関数まとめ [MyLibrary.h]
// Author :  
//
//=============================================================================
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

//=============================================================================
// プロトタイプ宣言
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, 
	int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Proj);	// スクリーン座標をワールド座標へ変換
D3DXVECTOR2 WorldToScreenPos(D3DXMATRIX WorldMatrix);

int CmpDescendf(const void *p, const void *q);		// floatの比較（降順）
int CmpAscendf(const void *p, const void *q);		// floatの比較（昇順）
int CmpDescend(const void *p, const void *q);		// intの比較（降順）
int CmpAscend(const void *p, const void *q);		// intの比較（昇順）

template<typename T> 
T clamp(T x, T low, T high)							// 上限、下限のチェック
{
	return min(max(x, low), high);
}

void ReadCsv(const char *data, vector<vector<int>> *MapVector);	// csvデータの読み込み
int LoopCountUp(int counter, int low, int high);				// カウントアップ関数
int LoopCountDown(int counter, int low, int high);				// カウントダウン関数

float RandomRange(float min, float max);						// floatのランダム値を取得（マイナス対応）

D3DXVECTOR3 GetSpherePosition(float Angle1, float Angle2, float Radius);	// 球体表面上の任意座標を取得する
D3DXVECTOR3 GetCirclePosition(float Angle1, float Radius);	// 円周上の任意座標を取得する(X,Y,0.0f)

#endif

