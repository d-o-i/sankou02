//=============================================================================
//
// リザルト画面でのタイマー表示 [ResultTimer.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultTimer.h"
#include "ResourceManager.h"

#define FONT_SIZE		(72)
#define FONT_INTERVAL	(80)

LPD3DXFONT ResultTimer::Font = { nullptr };

//=============================================================================
// コンストラクタ
//=============================================================================
ResultTimer::ResultTimer(DWORD _time, int _rank)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	startTime = 0;
	currentTime = 0;
	elapsedTime = _time;
	this->Rank = _rank;

	for (int i = 0; i < DIGIT_MAX; i++)
	{
		digit[i] = 0;
	}

	// ミリ秒
	digit[0] = (elapsedTime / 10) % 10;
	digit[1] = (elapsedTime / 100) % 10;

	// 秒
	digit[2] = (elapsedTime / 1000) % 10;
	digit[3] = (elapsedTime / 10000) % 6;

	// 分
	digit[4] = (elapsedTime / 60000) % 10;
	digit[5] = (elapsedTime / 600000) % 6;

	// ひとけたずつオブジェクトを生成
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i] = new ResultDigit(digit[i], i, _rank);
	}

	// 情報表示用フォントの設定
	D3DXCreateFont(Device, FONT_SIZE, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font);

}

//=============================================================================
// デストラクタ
//=============================================================================
ResultTimer::~ResultTimer()
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
void ResultTimer::Update()
{
	// ひとけたずつ更新
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void ResultTimer::Draw()
{
	// 分と秒間の':'
	RECT Min_Sec = { 238, 95 + this->Rank * FONT_INTERVAL, 333, 245 + this->Rank * FONT_INTERVAL };
	// 秒とミリ秒間の':'
	RECT Sec_ms = { 338, 95 + this->Rank * FONT_INTERVAL, 433, 245 + this->Rank * FONT_INTERVAL };
	Font->DrawText(NULL, ":", -1, &Min_Sec, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));
	Font->DrawText(NULL, ":", -1, &Sec_ms, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));
	
	// ひとけたずつ描画
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Draw();
	}
}

