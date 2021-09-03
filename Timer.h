//=============================================================================
//
// タイマー [Timer.h]
// Author : 
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "_2dobj.h"
#include "Digit.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Timer :
	public _2dobj
{
protected:
	Digit*		time[DIGIT_MAX];		// それぞれの桁を表示するオブジェクトのポインタ
	DWORD		startTime;				// ゲームの開始時間
	DWORD		currentTime;			// 現在の時間（= timeGetTime()）
	DWORD		elapsedTime;			// 経過時間（currentTime - startTime）
	DWORD		digit[DIGIT_MAX];		// 経過時間をそれぞれの桁に分解
	bool		use;
	LPD3DXFONT	Font = nullptr;			// テキスト用フォント

	void Debug();

public:
	Timer();
	~Timer();

	virtual void Update();
	virtual void Draw();

	void Start();			// タイマースタート
	void Stop();			// タイマーストップ
	DWORD Check();			// 経過時間の確認

};

#endif
