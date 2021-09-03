//=============================================================================
//
// パーティクルマネージャ [ParticleManager.h]
// Author :  
//
//=============================================================================
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "Confetti.h"
#include "UIParticle.h"
#include "Particle.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class ParticleManager
{
private:
	std::vector<Confetti*> confettiVector;		// 紙吹雪
	static std::vector<UIParticle*> uiparticleVector;	// UI用パーティクル
	static std::vector<Particle*> particleVector;		// 3D用パーティクル
	void Check();
#if _DEBUG
	void Debug();
#endif

public:
	ParticleManager();
	~ParticleManager();

	void Update();
	void Draw();

	void SetConfetti();
	static std::vector<UIParticle*> *GetUIParticle();
	static std::vector<Particle*> *GetParticle();
};

#endif
