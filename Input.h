//=============================================================================
//
// ���͏��� [input.h]
// Author : 
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// ----- �Q�[���p�b�h�֘A -----------------------------------------------------
#define GAMEPAD_MAX		(4)			// �Q�[���p�b�h�ő�ڑ���
#define DEADZONE		(2500)		// �W���C�X�e�B�b�N�̖����]�[����ݒ�
#define RANGE_MAX		(1000)		// �L���͈͂̍ő�l
#define RANGE_MIN		(-1000)		// �L���͈͂̍ŏ��l

// �L�[���蓖��
// �W���C�X�e�B�b�N
#define STICK_UP		0x00000001l	// �W���C�X�e�B�b�N��(.IY<0)
#define STICK_DOWN		0x00000002l	// �W���C�X�e�B�b�N��(.IY>0)
#define STICK_LEFT		0x00000004l // �W���C�X�e�B�b�N��(.IX<0)
#define STICK_RIGHT		0x00000008l // �W���C�X�e�B�b�N�E(.IX>0)
// �{�^��
#define BUTTON_A		0x00000010l	// A(PS4��)�{�^��(.rgbButton[0]&0x80)
#define BUTTON_B		0x00000020l	// B(PS4�~)�{�^��(.rgbButton[1]&0x80)
#define BUTTON_C		0x00000040l	// C(PS4��)�{�^��(.rgbButton[2]&0x80)
#define BUTTON_D		0x00000080l	// D(PS4��)�{�^��(.rgbButton[3]&0x80)
#define BUTTON_L1		0x000000F0l	// L1(PS4L1)�{�^��(.rgbButton[4]&0x80)
#define BUTTON_R1		0x00000100l	// R1(PS4R1)�{�^��(.rgbButton[5]&0x80)
#define BUTTON_L2		0x00000200l	// L2(PS4L2)�{�^��(.rgbButton[6]&0x80)
#define BUTTON_R2		0x00000400l	// R2(PS4R2)�{�^��(.rgbButton[7]&0x80)
#define BUTTON_SHARE	0x00000800l	// SHARE(PS4SHARE)�{�^��(.rgbButton[8]&0x80)
#define BUTTON_START	0x00000F00l	// START(PS4OPTION)�{�^��(.rgbButton{9}&0x80)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

// ----- �L�[�{�[�h�֘A -------------------------------------------------------
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// ----- �Q�[���p�b�h�֘A -----------------------------------------------------
bool IsButtonPressed(int padNo, DWORD button);
bool IsButtonTriggered(int padNo, DWORD button);
bool IsButtonRepeated(int padNo, DWORD button);
bool IsButtonReleased(int padNo, DWORD button);
float GetJoyStickAngle(int padNo);					// �W���C�X�e�B�b�N�̊p�x���擾����
float GetJoyStickVec(int padNo);					// �W���C�X�e�B�b�N�̃x�N�g�����擾����
int GetPadCount(void);								// �ڑ�����Ă���R���g���[���̐������o
int GetJoyStickLeftX(int padNo);					// ���W���C�X�e�B�b�N��X�l
int GetJoyStickLeftY(int padNo);					// ���W���C�X�e�B�b�N��Y�l

#endif