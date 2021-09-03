//=============================================================================
//
// ���͏��� [input.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX		(256)		// �L�[�ő吔
#define REPEAT_FRAME	(26)		// �Q�[���p�b�h�̂�s�[�Ƃ��L���ɂȂ�܂ł̃t���[����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitPad();	// �p�b�h������
					// BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UninitPad();	// �I��
void UpdatePad();	// �X�V

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// ----- �L�[�{�[�h�֘A -------------------------------------------------------
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̉�����Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��Ԃ�ێ����郏�[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

// ----- �Q�[���p�b�h�֘A -----------------------------------------------------
static LPDIRECTINPUTDEVICE8 pGamePad[GAMEPAD_MAX] = { NULL,NULL };	// �f�o�C�X�̃|�C���^
static DWORD PadState[GAMEPAD_MAX];			// �p�b�h��Ԃ�ۑ�
static DWORD PadTrigger[GAMEPAD_MAX];
static DWORD PadRepeat[GAMEPAD_MAX];
static DWORD PadRelease[GAMEPAD_MAX];
int PadRepeatCnt[GAMEPAD_MAX];				// �Q�[���p�b�h�̃��s�[�g�J�E���^
static int PadCount = 0;					// ���o�����Q�[���p�b�h�̐�
static float JoyStickAngle[GAMEPAD_MAX];	// �W���C�X�e�B�b�N�̊p�x��ۑ�
static float JoyStickVec[GAMEPAD_MAX];		// �W���C�X�e�B�b�N�̃x�N�g����ۑ�
static DIJOYSTATE2 dijs[GAMEPAD_MAX];		// �R���g���[���̏�Ԃ�ۑ�

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �Q�[���p�b�h�̏�����
	InitPad();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �Q�[���p�b�h�̏I������
	UninitPad();

	if(g_pDInput)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �Q�[���p�b�h�̍X�V
	UpdatePad();
}

//=============================================================================
// �L�[�{�[�h�̏���������
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState);
	if(SUCCEEDED(hr))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			g_aKeyStateRepeat[nCntKey] = g_aKeyStateTrigger[nCntKey];

			if(aKeyState[nCntKey])
			{
				g_aKeyStateRepeatCnt[nCntKey]++;
				if(g_aKeyStateRepeatCnt[nCntKey] >= 20)
				{
					g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCntKey] = 0;
				g_aKeyStateRepeat[nCntKey] = 0;
			}

			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

// ----- �Q�[���p�b�h�֘A -----------------------------------------------------
//=============================================================================
// �Q�[���p�b�h�����o����R�[���o�b�N�֐�
//=============================================================================
bool CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE ipddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(ipddi->guidInstance, &pGamePad[PadCount++], NULL);	// ���o������PadCount��������
	return DIENUM_CONTINUE;		// �f�o�C�X���
}

//=============================================================================
// ������
//=============================================================================
HRESULT InitPad()
{
	HRESULT result;

	PadCount = 0;
	// �p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);

	for (int i = 0; i < PadCount; i++)
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�A�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result)) return false;	// �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s
											// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
											//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
											//		if ( FAILED(result) )
											//			return false; // ���[�h�̐ݒ�Ɏ��s

											// ���̒l�͈̔͂�ݒ�
											// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
											// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
											// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)

		DIPROPRANGE diprg;								// �f�o�C�X���̃I�u�W�F�N�g�͈̔͂Ɋւ������ݒ肷��
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);				// diprg�\���̂̃T�C�Y��ݒ�
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// diprg.diph�\���̂̃T�C�Y��ݒ�
		diprg.diph.dwHow = DIPH_BYOFFSET;				// �����o�̉��ߕ��@���w��i�v���p�e�B���A�N�Z�X����Ă���I�u�W�F�N�g�̌��݂̃f�[�^�`���ւ̃I�t�Z�b�g�Ƃ���j
		diprg.lMin = RANGE_MIN;							// �I�u�W�F�N�g�̍ŏ��l
		diprg.lMax = RANGE_MAX;							// �I�u�W�F�N�g�̍ő�l
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�X�e�B�b�N�̖����]�[����ݒ肷��
		// �w�肷��l��10000�ɑ΂��鑊�Βl
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void UninitPad()
{
	for (int i = 0; i < GAMEPAD_MAX; i++)
	{
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}
}

//=============================================================================
// �X�V
//=============================================================================
void UpdatePad()
{
	HRESULT result;

	for (int i = 0; i < PadCount; i++)
	{
		DWORD lastPadState[GAMEPAD_MAX];
		lastPadState[i] = PadState[i];		// �O�t���[���ɓ��͂��ꂽ���̂�ۑ�
		PadState[i] = 0x00000000l;			// ������

		result = pGamePad[i]->Poll();		// �W���C�X�e�B�b�N�Ƀ|�[����������
		// �g�p�\���m�F
		if (FAILED(result))
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		// �R���g���[���̊e�{�^���̓��͊m�F
		// ----- �W���C�X�e�B�b�N ----------------------------------
		// ��
		if (dijs[i].lY < 0)		PadState[i] |= STICK_UP;
		// ��
		if (dijs[i].lY > 0)		PadState[i] |= STICK_DOWN;
		// ��
		if (dijs[i].lX < 0)		PadState[i] |= STICK_LEFT;
		// �E
		if (dijs[i].lX > 0)		PadState[i] |= STICK_RIGHT;

		// ----- �{�^�� --------------------------------------------
		// A(PS4��)
		if (dijs[i].rgbButtons[0] & 0x80)	PadState[i] |= BUTTON_A;
		// B(PS4�~)
		if (dijs[i].rgbButtons[1] & 0x80)	PadState[i] |= BUTTON_B;
		// C(PS4��)
		if (dijs[i].rgbButtons[2] & 0x80)	PadState[i] |= BUTTON_C;
		// D(PS4��)
		if (dijs[i].rgbButtons[3] & 0x80)	PadState[i] |= BUTTON_D;

		// L1(PS4L1)
		if (dijs[i].rgbButtons[4] & 0x80)	PadState[i] |= BUTTON_L1;
		// R1(PS4R1)
		if (dijs[i].rgbButtons[5] & 0x80)	PadState[i] |= BUTTON_R1;
		// L2(PS4L2)
		if (dijs[i].rgbButtons[6] & 0x80)	PadState[i] |= BUTTON_L2;
		// R2(PS4R2)
		if (dijs[i].rgbButtons[7] & 0x80)	PadState[i] |= BUTTON_R2;

		// SHARE(PS4SHARE)
		if (dijs[i].rgbButtons[8] & 0x80)	PadState[i] |= BUTTON_SHARE;
		// START(PS4OPTION)
		if (dijs[i].rgbButtons[9] & 0x80)	PadState[i] |= BUTTON_START;

		// Trigger�ݒ�
		PadTrigger[i] = (lastPadState[i] ^ PadState[i]) & PadState[i];	// �O�t���[���ƈ���Ă��āA����ON�̃{�^�������o

		// �����[�X�ݒ�
		PadRelease[i] = (lastPadState[i] ^ PadState[i]) & ~PadState[i];

		// ���s�[�g�ݒ�
		PadRepeat[i] = PadTrigger[i];

		// ���s�[�g��L���ɂ��邽�߂̊m�F
		if (PadState[i])
		{
			PadRepeatCnt[i]++;
			if (PadRepeatCnt[i] >= REPEAT_FRAME)
			{
				PadRepeat[i] = PadState[i];
			}
			else
			{
				PadRepeatCnt[i] = 0;
				PadRepeat[i] = 0;
			}
		}

		// �W���C�X�e�B�b�N�̊p�x�A�x�N�g����ۑ�
		JoyStickAngle[i] = atan2f((float)dijs[i].lY, (float)dijs[i].lX);
		D3DXVECTOR2 temp = D3DXVECTOR2((float)dijs[i].lX, (float)dijs[i].lY);
		JoyStickVec[i] = D3DXVec2Length(&temp);

#ifndef _DEBUG_
		//// ���o���ꂽ�R���g���[���̂݃f�o�b�O��\������
		//if (i < PadCount)
		//{
		//	PrintDebugProc("JoyStick[%d] X:%d Y:%d\n", i, dijs[i].lX, dijs[i].lY);
		//	PrintDebugProc("JoyStickAngle[%d] %f\n", i, JoyStickAngle[i]);
		//	PrintDebugProc("JoyStickVec[%d] %f\n", i, JoyStickVec[i]);
		//}
#endif
	}
}

//=============================================================================
// �Q�[���p�b�h�̃v���X��Ԃ����o����
//=============================================================================
bool IsButtonPressed(int padNo, DWORD button)
{
	return (button & PadState[padNo]);
}

//=============================================================================
// �Q�[���p�b�h�̃g���K�[��Ԃ����o����
//=============================================================================
bool IsButtonTriggered(int padNo, DWORD button)
{
	return (button & PadTrigger[padNo]);
}

//=============================================================================
// �Q�[���p�b�h�̃��s�[�g��Ԃ����o����
//=============================================================================
bool IsButtonRepeated(int padNo, DWORD button)
{
	return (button & PadRepeat[padNo]);
}

//=============================================================================
// �Q�[���p�b�h�̃����[�X��Ԃ����o����
//=============================================================================
bool IsButtonReleased(int padNo, DWORD button)
{
	return (button & PadRelease[padNo]);
}

//=============================================================================
// �W���C�X�e�B�b�N�̊p�x���擾����
//=============================================================================
float GetJoyStickAngle(int padNo)
{
	return JoyStickAngle[padNo];
}

//=============================================================================
// �W���C�X�e�B�b�N�̃x�N�g�����擾����
//=============================================================================
float GetJoyStickVec(int padNo)
{
	return JoyStickVec[padNo];
}

//=============================================================================
// �ڑ�����Ă���R���g���[���̐����擾
//=============================================================================
int GetPadCount(void)
{
	return PadCount;
}

//=============================================================================
// ���W���C�X�e�B�b�N��X�l
//=============================================================================
int GetJoyStickLeftX(int padNo)
{
	return (int)dijs[padNo].lX;
}

//=============================================================================
// ���W���C�X�e�B�b�N��Y�l
//=============================================================================
int GetJoyStickLeftY(int padNo)
{
	return (int)dijs[padNo].lY;
}