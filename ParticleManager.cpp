//=============================================================================
//
// �p�[�e�B�N���}�l�[�W�� [ParticleManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ParticleManager.h"
#include "DebugWindow.h"

std::vector<UIParticle*> ParticleManager::uiparticleVector;
std::vector<Particle*> ParticleManager::particleVector;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ParticleManager::ParticleManager()
{
	// �������m��
	confettiVector.reserve(500);
	uiparticleVector.reserve(500);
	particleVector.reserve(1000);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ParticleManager::~ParticleManager()
{
	// �x�N�^�[�̍폜
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

	// �e�N�X�`���̍폜
	UIParticle::ReleaseTexture();
	Confetti::ReleaseTexture();
	Particle::ReleaseTexture();
}

//=============================================================================
// �X�V
//=============================================================================
void ParticleManager::Update()
{
	// �g�p�m�F
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
// �`��
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
// �g�p�m�F
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
// �f�o�b�O
//=============================================================================
void ParticleManager::Debug()
{
	BeginDebugWindow("Particle");

	DebugText("Count:%d", particleVector.capacity());

	EndDebugWindow("Particle");
}

//=============================================================================
// ������̃Z�b�g
//=============================================================================
void ParticleManager::SetConfetti()
{
	confettiVector.push_back(new Confetti());
}

//=============================================================================
// UI�p�p�[�e�B�N���x�N�^�[�̃Q�b�^�[
//=============================================================================
std::vector<UIParticle*> *ParticleManager::GetUIParticle()
{
	return &uiparticleVector;
}

//=============================================================================
// 3D�p�p�[�e�B�N���x�N�^�[�̃Q�b�^�[
//=============================================================================
std::vector<Particle*> *ParticleManager::GetParticle()
{
	return &particleVector;
}