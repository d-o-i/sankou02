//=============================================================================
//
// �G�t�F�N�g�Ǘ��}�l�[�W�� [EffectManager.h]
// Author :  
//
//=============================================================================
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "Effect.h"
#include "Effect3D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class EffectManager
{
private:
	void Check();

public:
	EffectManager();
	~EffectManager();

	void Update();
	void Draw();

};

std::vector<Effect*> *GetEffectVector();
std::vector<Effect3D*> *GetEffect3DVector();

#endif
