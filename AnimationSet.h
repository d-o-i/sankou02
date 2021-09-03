//=============================================================================
//
// �A�j���[�V�����Z�b�g�N���X[AnimationSet.cpp]
// Author :  
//
//=============================================================================
#ifndef _ANIMATIONSET_H_
#define _ANIMATIONSET_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �N���X��`
//*****************************************************************************
typedef struct
{
	float	Time;			// Keyframe�̎���
	int		KeyType;		// Keyframe�̎��
}KEYDATA;

class ANIMATIONSET
{
private:
	ID3DXAnimationSet	*SetPtr = NULL;					// �A�j���[�V�����Z�b�g�̃|�C���^
	LPCSTR				SetName = NULL;					// �A�j���[�V�����Z�b�g�̖��O
	float				Speed = 0.0f;
	float				ShiftTime = 0.0f;				// �V�t�g����̂ɂ����鎞��
	float				CurWeightTime = 0.0f;			// ���݂̃E�F�C�g����

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
