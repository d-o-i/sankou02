//=============================================================================
//
// �^�C�}�[ [Timer.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Timer.h"
#include "DebugWindow.h"
#include "ResourceManager.h"

//=============================================================================
// �R���X�g���N�^
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

	// �ЂƂ������I�u�W�F�N�g�𐶐�
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i] = new Digit(i);
	}

	// ���\���p�t�H���g�̐ݒ�
	D3DXCreateFont(Device, 54, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Consolas"), &Font);

}

//=============================================================================
// �f�X�g���N�^
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
// �X�V
//=============================================================================
void Timer::Update()
{
	Debug();

	if (!use)
		return;

	currentTime = timeGetTime();
	elapsedTime = currentTime - startTime;	// �o�ߎ��Ԃ̎Z�o

	// �~���b
	digit[0] = (elapsedTime / 10) % 10;
	digit[1] = (elapsedTime / 100) % 10;

	// �b
	digit[2] = (elapsedTime / 1000) % 10;
	digit[3] = (elapsedTime / 10000) % 6;

	// ��
	digit[4] = (elapsedTime / 60000) % 10;
	digit[5] = (elapsedTime / 600000) % 6;

	// �ЂƂ������X�V
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Update(digit[i]);
	}
}

//=============================================================================
// �`��
//=============================================================================
void Timer::Draw()
{
	RECT rect = { 660, 105, 725, 155 };
	Font->DrawText(NULL, ":", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));
	rect = { 560, 105, 625, 155 };
	Font->DrawText(NULL, ":", -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_RGBA(255, 0, 0, 255));

	// �ЂƂ������`��
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		time[i]->Draw();
	}
}

//=============================================================================
// �X�^�[�g
//=============================================================================
void Timer::Start()
{
	startTime = timeGetTime();
	use = true;
}

//=============================================================================
// �X�g�b�v
//=============================================================================
void Timer::Stop()
{
	use = false;
}

//=============================================================================
// �o�ߎ��Ԃ̊m�F
//=============================================================================
DWORD Timer::Check()
{
	return elapsedTime;
}

//=============================================================================
// �f�o�b�O
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
