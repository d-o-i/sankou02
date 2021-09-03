//=============================================================================
//
// エフェクト処理 [Effect.h]
// Author :  
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

// エフェクトの種類
enum EffectNum{
	HitEffect,
	DeadEffect,
	RunEffect,
	ExplosionEffect,
	ItemEffect,
	Item1Effect,
	ChargeEffect,

	// エフェクトの個数
	EffectMax,
};

#define INFINITY_LOOP	(-1)	// エフェクトを無限ループさせる

#include "_2dobj.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Effect :
	public _2dobj
{
private:
	LPDIRECT3DVERTEXBUFFER9		D3DVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3					rot;
	D3DXVECTOR3					scl;
	int							CountAnim;
	int							AnimationCnt;
	int							TexNo;
	int							TexAnimNum;				// アニメーションの分割数
	int							xPattern, yPattern;		// 分割数
	int							loopnum;				// ループさせる回数(無限ループは-1)
	int							loopcnt;				// 現在のループ回数

	HRESULT MakeVertex();						// 頂点情報の作成
	void SetTexture(int cntPattern);			// テクスチャ座標の設定
	void SetVertex();							// 頂点座標の設定

	void Loop();								// エフェクトのループ処理

public:
	Effect(EffectNum num, D3DXVECTOR3 _pos, int _LoopNum);
	~Effect();

	void Update();
	void Draw();

	bool GetUse() { return use; };
	void SetUse(bool _use) { use = _use; };
};

#endif

