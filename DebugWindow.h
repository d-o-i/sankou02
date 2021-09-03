//=============================================================================
//
// �f�o�b�O�E�B���h�E [DebugWindow.h]
// Author :  
//
//=============================================================================
#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include "Main.h"

// ***********************************
// �v���g�^�C�v�錾
// ***********************************
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitDebugWindow(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
void UninitDebugWindow(int num);
void UpdateDebugWindow();
void DrawDebugWindow();

void SetActiveDebugWindow(bool state = true);						// �\���؂�ւ�

void BeginTimerCount();												// �^�C�}�[�v���J�n
double GetProgressTimerCount();										// �^�C�}�[�o�ߎ��Ԏ擾
void GetTimerCount(LARGE_INTEGER *ptr);								// �^�C�}�[�J�E���g�擾
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end);	// �o�ߎ��Ԏ擾

// ImGui
void BeginDebugWindow(const char *label);
void EndDebugWindow(const char *label);
void DebugText(const char *str, ...);
bool DebugButton(const char *label);
bool DebugButton(const std::string *label);
bool DebugSliderFloat(const char *label, float *adr, float min, float max);
bool DebugSliderFloat(const std::string *label, float *adr, float min, float max);
void DebugColorEditor(const char *label, float array[4]);
void DebugNewLine();
void DebugSameLine();
void DebugTreeExpantion(bool isOpen);
bool DebugTreePush(const char *label);
void DebugTreePop();
void DebugProgressBar(float fraction, const char *label, D3DXVECTOR2 size = D3DXVECTOR2(-1.0f, 0.0f));
void DebugInputVector3(const char *label, D3DXVECTOR3 *vec);
void DebugInputVector2(const char *label, D3DXVECTOR2 *vec);
void DebugInputFloat(const char *label, float *var);

#endif
