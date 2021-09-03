//=============================================================================
//
// タイマー [Timer.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Timer.h"
#include "DebugWindow.h"
#include "ResourceManager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Timer::Timer()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	startTime = 0;
	currentTime = 0;
	elapsedTime = 0;

	for (int i = 0; i < DIGIT_MAX; i++)
	{
		digit[i] = 0;
	}

	// ひとけたずつオブジェクトを生成
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i] = new Digit(i);
	}

	// 情報表示用フォントの設定
	D3DXCreateFont(Device, 54, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font);

}

//=============================================================================
// デストラクタ
//=============================================================================
Timer::~Timer()
{
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		SAFE_DELETE(time[i]);
	}

	SAFE_RELEASE(Font);
}

//=============================================================================
// 更新
//=============================================================================
void Timer::Update()
{
	Debug();

	if (!use)
		return;

	currentTime = timeGetTime();
	elapsedTime = currentTime - startTime;	// 経過時間の算出

	// ミリ秒
	digit[0] = (elapsedTime / 10) % 10;
	digit[1] = (elapsedTime / 100) % 10;

	// 秒
	digit[2] = (elapsedTime / 1000) % 10;
	digit[3] = (elapsedTime / 10000) % 6;

	// 分
	digit[4] = (elapsedTime / 60000) % 10;
	digit[5] = (elapsedTime / 600000) % 6;

	// ひとけたずつ更新
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Update(digit[i]);
	}
}

//=============================================================================
// 描画
//=============================================================================
void Timer::Draw()
{
	RECT rect = { 660, 105, 725, 155 };
	Font->DrawText(NULL, ":", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));
	rect = { 560, 105, 625, 155 };
	Font->DrawText(NULL, ":", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));

	// ひとけたずつ描画
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Draw();
	}
}

//=============================================================================
// スタート
//=============================================================================
void Timer::Start()
{
	startTime = timeGetTime();
	use = true;
}

//=============================================================================
// ストップ
//=============================================================================
void Timer::Stop()
{
	use = false;
}

//=============================================================================
// 経過時間の確認
//=============================================================================
DWORD Timer::Check()
{
	return elapsedTime;
}

//=============================================================================
// デバッグ
//=============================================================================
void Timer::Debug()
{
#if _DEBUG_
	BeginDebugWindow("Timer");

	DebugText("Start:%d Current:%d Elapsed:%d", startTime, currentTime, elapsedTime);
	DebugText("%d%d%d%d%d%d", digit[5], digit[4], digit[3], digit[2], digit[1], digit[0]);

	EndDebugWindow("Timer");
#endif
}
