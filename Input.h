//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ----- ゲームパッド関連 -----------------------------------------------------
#define GAMEPAD_MAX		(4)			// ゲームパッド最大接続数
#define DEADZONE		(2500)		// ジョイスティックの無効ゾーンを設定
#define RANGE_MAX		(1000)		// 有効範囲の最大値
#define RANGE_MIN		(-1000)		// 有効範囲の最小値

// キー割り当て
// ジョイスティック
#define STICK_UP		0x00000001l	// ジョイスティック上(.IY<0)
#define STICK_DOWN		0x00000002l	// ジョイスティック下(.IY>0)
#define STICK_LEFT		0x00000004l // ジョイスティック左(.IX<0)
#define STICK_RIGHT		0x00000008l // ジョイスティック右(.IX>0)
// ボタン
#define BUTTON_A		0x00000010l	// A(PS4□)ボタン(.rgbButton[0]&0x80)
#define BUTTON_B		0x00000020l	// B(PS4×)ボタン(.rgbButton[1]&0x80)
#define BUTTON_C		0x00000040l	// C(PS4○)ボタン(.rgbButton[2]&0x80)
#define BUTTON_D		0x00000080l	// D(PS4△)ボタン(.rgbButton[3]&0x80)
#define BUTTON_L1		0x000000F0l	// L1(PS4L1)ボタン(.rgbButton[4]&0x80)
#define BUTTON_R1		0x00000100l	// R1(PS4R1)ボタン(.rgbButton[5]&0x80)
#define BUTTON_L2		0x00000200l	// L2(PS4L2)ボタン(.rgbButton[6]&0x80)
#define BUTTON_R2		0x00000400l	// R2(PS4R2)ボタン(.rgbButton[7]&0x80)
#define BUTTON_SHARE	0x00000800l	// SHARE(PS4SHARE)ボタン(.rgbButton[8]&0x80)
#define BUTTON_START	0x00000F00l	// START(PS4OPTION)ボタン(.rgbButton{9}&0x80)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

// ----- キーボード関連 -------------------------------------------------------
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// ----- ゲームパッド関連 -----------------------------------------------------
bool IsButtonPressed(int padNo, DWORD button);
bool IsButtonTriggered(int padNo, DWORD button);
bool IsButtonRepeated(int padNo, DWORD button);
bool IsButtonReleased(int padNo, DWORD button);
float GetJoyStickAngle(int padNo);					// ジョイスティックの角度を取得する
float GetJoyStickVec(int padNo);					// ジョイスティックのベクトルを取得する
int GetPadCount(void);								// 接続されているコントローラの数を検出
int GetJoyStickLeftX(int padNo);					// 左ジョイスティックのX値
int GetJoyStickLeftY(int padNo);					// 左ジョイスティックのY値

#endif