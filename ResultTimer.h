//=============================================================================
//
// ���U���g��ʂł̃^�C�}�[�\�� [ResultTimer.cpp]
// Author : 
//
//=============================================================================
#ifndef _RESULTTIMER_H_
#define  RESULTTIMER_H_

#include "Timer.h"
#include "ResultDigit.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ResultTimer :
	public Timer
{
private:
	ResultDigit*		time[DIGIT_MAX];	// ���ꂼ��̌���\������I�u�W�F�N�g�̃|�C���^
	int					Rank;				// �����N
	static LPD3DXFONT	Font;				// �e�L�X�g�p�t�H���g

public:
	ResultTimer(DWORD _time, int _rank);
	~ResultTimer();

	void Update()override;
	void Draw()override;
};

#endif
