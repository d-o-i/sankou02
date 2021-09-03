//=============================================================================
//
// アニメーションセットクラス[AnimationSet.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "AnimationSet.h"


//=====================================================================================================
// コンストラクタ
//=====================================================================================================
ANIMATIONSET::ANIMATIONSET()
{

}

//=====================================================================================================
// デストラクタ
//=====================================================================================================
ANIMATIONSET::~ANIMATIONSET()
{
	//SAFE_RELEASE(this->SetPtr);
}

void ANIMATIONSET::SetData(LPCSTR Name, ID3DXAnimationSet *SetPtr, float Speed, float ShiftTime, float CurWeightTime)
{
	this->SetName = Name;
	this->SetPtr = SetPtr;
	this->Speed = Speed;
	this->ShiftTime = ShiftTime;
	this->CurWeightTime = CurWeightTime;
}

void ANIMATIONSET::SetAnimSetPtr(LPD3DXANIMATIONCONTROLLER AnimCtrl)
{
	char Message[256] = { NULL };

	if (FAILED(AnimCtrl->GetAnimationSetByName(this->SetName, &this->SetPtr)))
	{
		sprintf_s(Message, "Can't Find %s AnimationSet Failed！", SetName);
		MessageBox(0, Message, "Error", 0);
	}
}
