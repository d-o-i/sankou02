//=============================================================================
//
// エフェクト管理マネージャ [EffectManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "EffectManager.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static std::vector<Effect*> EffectVector;
static std::vector<Effect3D*> Effect3DVector;

//=====================================================================================================
// コンストラクタ
//=====================================================================================================
EffectManager::EffectManager()
{
	// ベクターメモリの確保
	EffectVector.reserve(EffectMax);
	EffectVector.reserve(EffectMax);
}

//=====================================================================================================
// デストラクタ
//=====================================================================================================
EffectManager::~EffectManager()
{
	for (auto &effect : EffectVector)
	{
		SAFE_DELETE(effect);
	}
	EffectVector.clear();
	ReleaseVector(EffectVector);

	for (auto &effect3D : Effect3DVector)
	{
		SAFE_DELETE(effect3D);
	}
	Effect3DVector.clear();
	ReleaseVector(Effect3DVector);
}

//=====================================================================================================
// 更新
//=====================================================================================================
void EffectManager::Update()
{
	Check();

	for (auto &effect3D : Effect3DVector)
	{
		effect3D->Update();
	}

	for (auto &effect : EffectVector)
	{
		effect->Update();
	}

}

//=====================================================================================================
// 描画
//=====================================================================================================
void EffectManager::Draw()
{
	for (auto &effect3D : Effect3DVector)
	{
		effect3D->Draw();
	}

	for (auto &effect : EffectVector)
	{
		effect->Draw();
	}

}

//=====================================================================================================
// 使用確認
//=====================================================================================================
void EffectManager::Check()
{
	for (auto effect = EffectVector.begin(); effect != EffectVector.end();)
	{
		if ((*effect)->GetUse() == false)
		{
			SAFE_DELETE((*effect));
			effect = EffectVector.erase(effect);
		}
		else
		{
			effect++;
		}
	}

	for (auto effect3D = Effect3DVector.begin(); effect3D != Effect3DVector.end();)
	{
		if ((*effect3D)->GetUse() == false)
		{
			SAFE_DELETE((*effect3D));
			effect3D = Effect3DVector.erase(effect3D);
		}
		else
		{
			effect3D++;
		}
	}
}

//=====================================================================================================
// ゲッター
//=====================================================================================================
std::vector<Effect*> *GetEffectVector()
{ 
	return &EffectVector; 
}

//=====================================================================================================
// ゲッター
//=====================================================================================================
std::vector<Effect3D*> *GetEffect3DVector()
{
	return &Effect3DVector;
}