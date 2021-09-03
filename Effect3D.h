//=============================================================================
//
// 3Dビルボードエフェクト処理 [Effect3D.h]
// Author :  
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "Billboard.h"
#include "Effect.h"

// エフェクトの種類
enum Effect3DNum {
	HitEffect3D,
	DeadEffect3D,
	RunEffect3D,
	ExplosionEffect3D,
	ItemEffect3D,
	Item1Effect3D,
	ChargeEffect3D,

	// エフェクトの個数
	EffectMax3D,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Effect3D :
	public Billboard
{
private:
	bool						use;
	int							CountAnim;
	int							PatternAnim;			// アニメーションパターン
	int							AnimationCnt;
	int							TexNo;
	int							TexAnimNum;				// アニメーションの分割数
	int							xPattern, yPattern;		// 分割数
	D3DXVECTOR3					size;					// 表示サイズ
	int							loopnum;				// ループさせる回数(無限ループは-1)
	int							loopcnt;				// 現在のループ回数

	HRESULT MakeVertex();
	void SetTexture();

	void Loop();

public:
	Effect3D(int Effect3DNum, D3DXVECTOR3 _pos, int _LoopNum);
	~Effect3D();

	void Update()override;
	void Draw()override;

	bool GetUse() { return this->use; };
	void SetUse(bool use) { this->use = use; };
};

#endif
