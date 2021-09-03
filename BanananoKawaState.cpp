//=============================================================================
//
// �A�C�e���̏�ԊǗ��i�o�i�i�̔�j[BanananoKawaState.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "BanananoKawaState.h"
#include "Input.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
BanananoKawaState::BanananoKawaState(Item *owner) : owner_(owner)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
BanananoKawaState::~BanananoKawaState()
{
	owner_ = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void BanananoKawaState::Update()
{
	//if (IsButtonTriggered(owner_->GetPlayer()->GetCtrlNum(), BUTTON_D) || GetKeyboardTrigger(DIK_I))
	//{
		D3DXVECTOR3 setpos = owner_->GetPlayer()->GetPos();
		owner_->GetPlayer()->GetFieldItemManager()->Set(NumKawa, setpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		// �A�C�e�����g�p
		owner_->Reset();
		// PlaySound(�A�C�e���g�p)
	//}
}

//=============================================================================
// �L���ɂ���Ƃ������g��
//=============================================================================
void BanananoKawaState::Start()
{

}