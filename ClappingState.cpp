//=============================================================================
//
// �v���C���[�̏�ԊǗ��i���蒆�j[ClappingState.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ClappingState.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ClappingState::ClappingState(Player* owner) : owner_(owner)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ClappingState::~ClappingState()
{
	owner_ = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void ClappingState::Update(int AnimCurtID)
{
}