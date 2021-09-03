//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :  
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// �T�E���h�t�@�C��
typedef enum
{
	// BGM�i���y�j
	BGM_TITLE,
	BGM_TUTORIAL,
	BGM_CHARSEL,
	BGM_TRAINING,
	//BGM_BATTLE,
	// SE�i�T�E���h�G�t�F�N�g�j
	SE_CHOICE,
	SE_SELECT,
	SE_PICITEM,
	SE_EXP,
	SE_USEITEM,
	SE_PAINT,
	SE_COUNT,
	SE_ITEMRU,
	//SE_EFFECT0,
	//SE_CUTIN0,
	//SE_COUNTDOWN,

	//-----------------------------------------------------------------------------
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
