//=============================================================================
//
// �p�[�e�B�N���}�l�[�W�� [ParticleManager.h]
// Author :  
//
//=============================================================================
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "Confetti.h"
#include "UIParticle.h"
#include "Particle.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ParticleManager
{
private:
	std::vector<Confetti*> confettiVector;		// ������
	static std::vector<UIParticle*> uiparticleVector;	// UI�p�p�[�e�B�N��
	static std::vector<Particle*> particleVector;		// 3D�p�p�[�e�B�N��
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
