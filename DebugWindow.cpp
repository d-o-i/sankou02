//=============================================================================
//
// �f�o�b�O�E�B���h�E [DebugWindow.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "DebugWindow.h"

/**************************************
�}�N����`
***************************************/
#define INTERBAL_GETTIMER		(20)

/**************************************
�O���[�o���ϐ�
***************************************/
LARGE_INTEGER StartTime;
//static unsigned int cntFrame = 0;
static bool enableDraw = true;		// �f�o�b�O�E�B���h�E�\����

/**************************************
�v���g�^�C�v�錾
***************************************/
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// �f�o�b�O�E�B���h�E�p�R�[���o�b�N
//=============================================================================
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

//=============================================================================
// ������
//=============================================================================
void InitDebugWindow(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui������
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pDevice);

	// �E�B���h�E�̐F
	ImGui::StyleColorsLight();
}

//=============================================================================
// �I��
//=============================================================================
void UninitDebugWindow(int num)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
// �X�V
//=============================================================================
void UpdateDebugWindow()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	// �o�߃t���[���̃J�E���g
	//cntFrame++;
}

//=============================================================================
// �`��
//=============================================================================
void DrawDebugWindow()
{
	// ���݂��Ȃ�
	if (!enableDraw)
	{
		ImGui::EndFrame();
		return;
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// �\���؂�ւ�����
//=============================================================================
void SetActiveDebugWindow(bool state)
{
	enableDraw = state;
}

//=============================================================================
// �^�C�}�[�v���J�n
//=============================================================================
void BeginTimerCount()
{
	QueryPerformanceCounter(&StartTime);
}

//=============================================================================
// �^�C�}�[�o�ߎ��Ԏ擾
//=============================================================================
double GetProgressTimerCount()
{
	// �^�C�}�[�̎��g�����擾
	LARGE_INTEGER frequencyTimer;
	QueryPerformanceFrequency(&frequencyTimer);

	// �J�E���g�擾
	LARGE_INTEGER timeCurrent;
	QueryPerformanceCounter(&timeCurrent);

	// �v���J�n����̌o�ߎ��Ԃ��v�Z
	LONGLONG span = timeCurrent.QuadPart - StartTime.QuadPart;
	return (double)span * 1000 / (double)frequencyTimer.QuadPart;
}

//=============================================================================
// �^�C�}�[�J�E���g�擾
//=============================================================================
void GetTimerCount(LARGE_INTEGER *ptr)
{
	//if (cntFrame % INTERBAL_GETTIMER != 0)
	//	return;

	//QueryPerformanceCounter(ptr);
}

//=============================================================================
// �o�ߎ��Ԏ擾
//=============================================================================
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end)
{
	// �^�C�}�[�̎��g���擾
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// �o�ߎ��Ԃ��v�Z
	LONGLONG span = end.QuadPart - start.QuadPart;
	return (double)span * 1000.0f / (double)frequency.QuadPart;
}
	
//=============================================================================
// �J�n
//=============================================================================
void BeginDebugWindow(const char *label)
{
	ImGui::Begin(label);
	ImGui::SetWindowFontScale(1.3f);
}

//=============================================================================
// �I��
//=============================================================================
void EndDebugWindow(const char *label)
{
	ImGui::End();
}

//=============================================================================
// �e�L�X�g�\��
//=============================================================================
void DebugText(const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	ImGui::TextV(str, ap);
	va_end(ap);
}

//=============================================================================
// �f�o�b�O�{�^���\��
//=============================================================================
bool DebugButton(const char *label)
{
	return ImGui::Button(label);
}

//=============================================================================
// �f�o�b�O�{�^���\��
//=============================================================================
bool DebugButton(const std::string *label)
{
	return ImGui::Button(label->c_str());
}

//=============================================================================
// �X���C�_�[����
//=============================================================================
bool DebugSliderFloat(const char *label, float *adr, float min, float max)
{
	return ImGui::SliderFloat(label, adr, min, max);
}

//=============================================================================
// �X���C�_�[����
//=============================================================================
bool DebugSliderFloat(const std::string *label, float *adr, float min, float max)
{
	return ImGui::SliderFloat(label->c_str(), adr, min, max);
}

//=============================================================================
// �J���[�s�b�J�[
//=============================================================================
void DebugColorEditor(const char *label, float array[4])
{
	ImGui::ColorEdit4(label, array);
}

//=============================================================================
// ���s
//=============================================================================
void DebugNewLine()
{
	ImGui::NewLine();
}

//=============================================================================
// �A���s
//=============================================================================
void DebugSameLine()
{
	ImGui::SameLine();
}

//=============================================================================
// �c���[�\���W�J
//=============================================================================
void DebugTreeExpantion(bool isOpen)
{
	ImGui::SetNextTreeNodeOpen(isOpen, ImGuiSetCond_Once);
}

//=============================================================================
// �c���[�Ƀv�b�V��
//=============================================================================
bool DebugTreePush(const char *label)
{
	return ImGui::TreeNode(label);
}

//=============================================================================
// �c���[����|�b�v
//=============================================================================
void DebugTreePop()
{
	ImGui::TreePop();
}

//=============================================================================
// �v���O���X�o�[
//=============================================================================
void DebugProgressBar(float fraction, const char *label, D3DXVECTOR2 size)
{
	ImGui::ProgressBar(fraction, ImVec2(size.x, size.y), label);
}

//=============================================================================
// 3�����x�N�g������
//=============================================================================
void DebugInputVector3(const char *label, D3DXVECTOR3 *vec)
{
	ImGui::InputFloat3(label, (float*)vec);
}

//=============================================================================
// 2�����x�N�g������
//=============================================================================
void DebugInputVector2(const char *label, D3DXVECTOR2 *vec)
{
	ImGui::InputFloat2(label, (float*)vec);
}

//=============================================================================
// float�^����
//=============================================================================
void DebugInputFloat(const char *label, float *var)
{
	ImGui::InputFloat(label, var);
}
