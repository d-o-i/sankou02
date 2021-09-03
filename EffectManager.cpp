//=============================================================================
//
// �G�t�F�N�g�Ǘ��}�l�[�W�� [EffectManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "EffectManager.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static std::vector<Effect*> EffectVector;
static std::vector<Effect3D*> Effect3DVector;

//=====================================================================================================
// �R���X�g���N�^
//=====================================================================================================
EffectManager::EffectManager()
{
	// �x�N�^�[�������̊m��
	EffectVector.reserve(EffectMax);
	EffectVector.reserve(EffectMax);
}

//=====================================================================================================
// �f�X�g���N�^
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
// �X�V
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
// �`��
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
// �g�p�m�F
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
// �Q�b�^�[
//=====================================================================================================
std::vector<Effect*> *GetEffectVector()
{ 
	return &EffectVector; 
}

//=====================================================================================================
// �Q�b�^�[
//=====================================================================================================
std::vector<Effect3D*> *GetEffect3DVector()
{
	return &Effect3DVector;
}