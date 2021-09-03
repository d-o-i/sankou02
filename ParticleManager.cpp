//=============================================================================
//
// パーティクルマネージャ [ParticleManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ParticleManager.h"
#include "DebugWindow.h"

std::vector<UIParticle*> ParticleManager::uiparticleVector;
std::vector<Particle*> ParticleManager::particleVector;

//=============================================================================
// コンストラクタ
//=============================================================================
ParticleManager::ParticleManager()
{
	// メモリ確保
	confettiVector.reserve(500);
	uiparticleVector.reserve(500);
	particleVector.reserve(1000);
}

//=============================================================================
// デストラクタ
//=============================================================================
ParticleManager::~ParticleManager()
{
	// ベクターの削除
	for (auto &Particle : confettiVector)
	{
		SAFE_DELETE(Particle);
	}
	confettiVector.clear();
	ReleaseVector(confettiVector);

	for (auto &Particle : uiparticleVector)
	{
		SAFE_DELETE(Particle);
	}
	uiparticleVector.clear();
	ReleaseVector(uiparticleVector);

	for (auto &Particle : particleVector)
	{
		SAFE_DELETE(Particle);
	}
	particleVector.clear();
	ReleaseVector(particleVector);

	// テクスチャの削除
	UIParticle::ReleaseTexture();
	Confetti::ReleaseTexture();
	Particle::ReleaseTexture();
}

//=============================================================================
// 更新
//=============================================================================
void ParticleManager::Update()
{
	// 使用確認
	Check();

	for (auto &Confetti : confettiVector)
	{
		Confetti->Update();
	}

	for (auto &Particle : uiparticleVector)
	{
		Particle->Update();
	}

	for (auto &Particle : particleVector)
	{
		Particle->Update();
	}

#if _DEBUG
	Debug();
#endif
}

//=============================================================================
// 描画
//=============================================================================
void ParticleManager::Draw()
{
	for (auto &Confetti : confettiVector)
	{
		Confetti->Draw();
	}

	for (auto &Particle : uiparticleVector)
	{
		Particle->Draw();
	}

	for (auto &Particle : particleVector)
	{
		Particle->Draw();
	}
}

//=============================================================================
// 使用確認
//=============================================================================
void ParticleManager::Check()
{
	for (auto Confetti = confettiVector.begin(); Confetti != confettiVector.end();)
	{
		if ((*Confetti)->GetUse() == false)
		{
			SAFE_DELETE((*Confetti));
			Confetti = confettiVector.erase(Confetti);
		}
		else
		{
			Confetti++;
		}
	}

	for (auto Particle = uiparticleVector.begin(); Particle != uiparticleVector.end();)
	{
		if ((*Particle)->GetUse() == false)
		{
			SAFE_DELETE((*Particle));
			Particle = uiparticleVector.erase(Particle);
		}
		else
		{
			Particle++;
		}
	}

	for (auto Particle = particleVector.begin(); Particle != particleVector.end();)
	{
		if ((*Particle)->GetUse() == false)
		{
			SAFE_DELETE((*Particle));
			Particle = particleVector.erase(Particle);
		}
		else
		{
			Particle++;
		}
	}

}

//=============================================================================
// デバッグ
//=============================================================================
void ParticleManager::Debug()
{
	BeginDebugWindow("Particle");

	DebugText("Count:%d", particleVector.capacity());

	EndDebugWindow("Particle");
}

//=============================================================================
// 紙吹雪のセット
//=============================================================================
void ParticleManager::SetConfetti()
{
	confettiVector.push_back(new Confetti());
}

//=============================================================================
// UI用パーティクルベクターのゲッター
//=============================================================================
std::vector<UIParticle*> *ParticleManager::GetUIParticle()
{
	return &uiparticleVector;
}

//=============================================================================
// 3D用パーティクルベクターのゲッター
//=============================================================================
std::vector<Particle*> *ParticleManager::GetParticle()
{
	return &particleVector;
}