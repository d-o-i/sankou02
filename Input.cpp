//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX		(256)		// キー最大数
#define REPEAT_FRAME	(26)		// ゲームパッドのりピーとが有効になるまでのフレーム数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitPad();	// パッド初期化
					// BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UninitPad();	// 終了
void UpdatePad();	// 更新

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// ----- キーボード関連 -------------------------------------------------------
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの押下状態を保持するワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー状態を保持するワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース状態を保持するワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

// ----- ゲームパッド関連 -----------------------------------------------------
static LPDIRECTINPUTDEVICE8 pGamePad[GAMEPAD_MAX] = { NULL,NULL };	// デバイスのポインタ
static DWORD PadState[GAMEPAD_MAX];			// パッド状態を保存
static DWORD PadTrigger[GAMEPAD_MAX];
static DWORD PadRepeat[GAMEPAD_MAX];
static DWORD PadRelease[GAMEPAD_MAX];
int PadRepeatCnt[GAMEPAD_MAX];				// ゲームパッドのリピートカウンタ
static int PadCount = 0;					// 検出したゲームパッドの数
static float JoyStickAngle[GAMEPAD_MAX];	// ジョイスティックの角度を保存
static float JoyStickVec[GAMEPAD_MAX];		// ジョイスティックのベクトルを保存
static DIJOYSTATE2 dijs[GAMEPAD_MAX];		// コントローラの状態を保存

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// ゲームパッドの初期化
	InitPad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// ゲームパッドの終了処理
	UninitPad();

	if(g_pDInput)
	{// DirectInputオブジェクトの開放
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// ゲームパッドの更新
	UpdatePad();
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{// デバイスオブジェクトの開放
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
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
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

// ----- ゲームパッド関連 -----------------------------------------------------
//=============================================================================
// ゲームパッドを検出するコールバック関数
//=============================================================================
bool CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE ipddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(ipddi->guidInstance, &pGamePad[PadCount++], NULL);	// 検出数だけPadCountが増える
	return DIENUM_CONTINUE;		// デバイスを列挙
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitPad()
{
	HRESULT result;

	PadCount = 0;
	// パッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);

	for (int i = 0; i < PadCount; i++)
	{
		// ジョイスティック用のデータ、フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result)) return false;	// データフォーマットの設定に失敗
											// モードを設定（フォアグラウンド＆非排他モード）
											//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
											//		if ( FAILED(result) )
											//			return false; // モードの設定に失敗

											// 軸の値の範囲を設定
											// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
											// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
											// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)

		DIPROPRANGE diprg;								// デバイス内のオブジェクトの範囲に関する情報を設定する
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);				// diprg構造体のサイズを設定
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// diprg.diph構造体のサイズを設定
		diprg.diph.dwHow = DIPH_BYOFFSET;				// メンバの解釈方法を指定（プロパティがアクセスされているオブジェクトの現在のデータ形式へのオフセットとする）
		diprg.lMin = RANGE_MIN;							// オブジェクトの最小値
		diprg.lMax = RANGE_MAX;							// オブジェクトの最大値
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイスティックの無効ゾーンを設定する
		// 指定する値は10000に対する相対値
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return true;
}

//=============================================================================
// 終了
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
// 更新
//=============================================================================
void UpdatePad()
{
	HRESULT result;

	for (int i = 0; i < PadCount; i++)
	{
		DWORD lastPadState[GAMEPAD_MAX];
		lastPadState[i] = PadState[i];		// 前フレームに入力されたものを保存
		PadState[i] = 0x00000000l;			// 初期化

		result = pGamePad[i]->Poll();		// ジョイスティックにポールをかける
		// 使用可能か確認
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

		// コントローラの各ボタンの入力確認
		// ----- ジョイスティック ----------------------------------
		// 上
		if (dijs[i].lY < 0)		PadState[i] |= STICK_UP;
		// 下
		if (dijs[i].lY > 0)		PadState[i] |= STICK_DOWN;
		// 左
		if (dijs[i].lX < 0)		PadState[i] |= STICK_LEFT;
		// 右
		if (dijs[i].lX > 0)		PadState[i] |= STICK_RIGHT;

		// ----- ボタン --------------------------------------------
		// A(PS4□)
		if (dijs[i].rgbButtons[0] & 0x80)	PadState[i] |= BUTTON_A;
		// B(PS4×)
		if (dijs[i].rgbButtons[1] & 0x80)	PadState[i] |= BUTTON_B;
		// C(PS4○)
		if (dijs[i].rgbButtons[2] & 0x80)	PadState[i] |= BUTTON_C;
		// D(PS4△)
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

		// Trigger設定
		PadTrigger[i] = (lastPadState[i] ^ PadState[i]) & PadState[i];	// 前フレームと違っていて、現在ONのボタンを検出

		// リリース設定
		PadRelease[i] = (lastPadState[i] ^ PadState[i]) & ~PadState[i];

		// リピート設定
		PadRepeat[i] = PadTrigger[i];

		// リピートを有効にするための確認
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

		// ジョイスティックの角度、ベクトルを保存
		JoyStickAngle[i] = atan2f((float)dijs[i].lY, (float)dijs[i].lX);
		D3DXVECTOR2 temp = D3DXVECTOR2((float)dijs[i].lX, (float)dijs[i].lY);
		JoyStickVec[i] = D3DXVec2Length(&temp);

#ifndef _DEBUG_
		//// 検出されたコントローラのみデバッグを表示する
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
// ゲームパッドのプレス状態を検出する
//=============================================================================
bool IsButtonPressed(int padNo, DWORD button)
{
	return (button & PadState[padNo]);
}

//=============================================================================
// ゲームパッドのトリガー状態を検出する
//=============================================================================
bool IsButtonTriggered(int padNo, DWORD button)
{
	return (button & PadTrigger[padNo]);
}

//=============================================================================
// ゲームパッドのリピート状態を検出する
//=============================================================================
bool IsButtonRepeated(int padNo, DWORD button)
{
	return (button & PadRepeat[padNo]);
}

//=============================================================================
// ゲームパッドのリリース状態を検出する
//=============================================================================
bool IsButtonReleased(int padNo, DWORD button)
{
	return (button & PadRelease[padNo]);
}

//=============================================================================
// ジョイスティックの角度を取得する
//=============================================================================
float GetJoyStickAngle(int padNo)
{
	return JoyStickAngle[padNo];
}

//=============================================================================
// ジョイスティックのベクトルを取得する
//=============================================================================
float GetJoyStickVec(int padNo)
{
	return JoyStickVec[padNo];
}

//=============================================================================
// 接続されているコントローラの数を取得
//=============================================================================
int GetPadCount(void)
{
	return PadCount;
}

//=============================================================================
// 左ジョイスティックのX値
//=============================================================================
int GetJoyStickLeftX(int padNo)
{
	return (int)dijs[padNo].lX;
}

//=============================================================================
// 左ジョイスティックのY値
//=============================================================================
int GetJoyStickLeftY(int padNo)
{
	return (int)dijs[padNo].lY;
}