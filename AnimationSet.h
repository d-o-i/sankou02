//=============================================================================
//
// アニメーションセットクラス[AnimationSet.cpp]
// Author :  
//
//=============================================================================
#ifndef _ANIMATIONSET_H_
#define _ANIMATIONSET_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// クラス定義
//*****************************************************************************
typedef struct
{
	float	Time;			// Keyframeの時間
	int		KeyType;		// Keyframeの種類
}KEYDATA;

class ANIMATIONSET
{
private:
	ID3DXAnimationSet	*SetPtr = NULL;					// アニメーションセットのポインタ
	LPCSTR				SetName = NULL;					// アニメーションセットの名前
	float				Speed = 0.0f;
	float				ShiftTime = 0.0f;				// シフトするのにかかる時間
	float				CurWeightTime = 0.0f;			// 現在のウェイト時間

public:
	ANIMATIONSET();
	~ANIMATIONSET();
	ID3DXAnimationSet* GetAnimSetPtr(void) { return this->SetPtr; };
	float GetSpeed(void) { return this->Speed; };
	float GetShiftTime(void) { return this->ShiftTime; };
	float GetCurWeightTime(void) { return this->CurWeightTime; };
	double GetPeriod(void) { return this->SetPtr->GetPeriod(); };
	LPCSTR GetSetName(void) { return this->SetName; };

	void SetData(LPCSTR Name, ID3DXAnimationSet *SetPtr, float Speed, float ShiftTime, float CurWeightTime);
	void SetCurWeightTime(float Time) { this->CurWeightTime = Time; };
	void SetAnimSetPtr(LPD3DXANIMATIONCONTROLLER AnimCtrl);
};

#endif
