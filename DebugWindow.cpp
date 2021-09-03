//=============================================================================
//
// デバッグウィンドウ [DebugWindow.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "DebugWindow.h"

/**************************************
マクロ定義
***************************************/
#define INTERBAL_GETTIMER		(20)

/**************************************
グローバル変数
***************************************/
LARGE_INTEGER StartTime;
//static unsigned int cntFrame = 0;
static bool enableDraw = true;		// デバッグウィンドウ表示可否

/**************************************
プロトタイプ宣言
***************************************/
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// デバッグウィンドウ用コールバック
//=============================================================================
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

//=============================================================================
// 初期化
//=============================================================================
void InitDebugWindow(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui初期化
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pDevice);

	// ウィンドウの色
	ImGui::StyleColorsLight();
}

//=============================================================================
// 終了
//=============================================================================
void UninitDebugWindow(int num)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
// 更新
//=============================================================================
void UpdateDebugWindow()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	// 経過フレームのカウント
	//cntFrame++;
}

//=============================================================================
// 描画
//=============================================================================
void DrawDebugWindow()
{
	// 存在しない
	if (!enableDraw)
	{
		ImGui::EndFrame();
		return;
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// 表示切り替え処理
//=============================================================================
void SetActiveDebugWindow(bool state)
{
	enableDraw = state;
}

//=============================================================================
// タイマー計測開始
//=============================================================================
void BeginTimerCount()
{
	QueryPerformanceCounter(&StartTime);
}

//=============================================================================
// タイマー経過時間取得
//=============================================================================
double GetProgressTimerCount()
{
	// タイマーの周波数を取得
	LARGE_INTEGER frequencyTimer;
	QueryPerformanceFrequency(&frequencyTimer);

	// カウント取得
	LARGE_INTEGER timeCurrent;
	QueryPerformanceCounter(&timeCurrent);

	// 計測開始からの経過時間を計算
	LONGLONG span = timeCurrent.QuadPart - StartTime.QuadPart;
	return (double)span * 1000 / (double)frequencyTimer.QuadPart;
}

//=============================================================================
// タイマーカウント取得
//=============================================================================
void GetTimerCount(LARGE_INTEGER *ptr)
{
	//if (cntFrame % INTERBAL_GETTIMER != 0)
	//	return;

	//QueryPerformanceCounter(ptr);
}

//=============================================================================
// 経過時間取得
//=============================================================================
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end)
{
	// タイマーの周波数取得
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// 経過時間を計算
	LONGLONG span = end.QuadPart - start.QuadPart;
	return (double)span * 1000.0f / (double)frequency.QuadPart;
}
	
//=============================================================================
// 開始
//=============================================================================
void BeginDebugWindow(const char *label)
{
	ImGui::Begin(label);
	ImGui::SetWindowFontScale(1.3f);
}

//=============================================================================
// 終了
//=============================================================================
void EndDebugWindow(const char *label)
{
	ImGui::End();
}

//=============================================================================
// テキスト表示
//=============================================================================
void DebugText(const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	ImGui::TextV(str, ap);
	va_end(ap);
}

//=============================================================================
// デバッグボタン表示
//=============================================================================
bool DebugButton(const char *label)
{
	return ImGui::Button(label);
}

//=============================================================================
// デバッグボタン表示
//=============================================================================
bool DebugButton(const std::string *label)
{
	return ImGui::Button(label->c_str());
}

//=============================================================================
// スライダー処理
//=============================================================================
bool DebugSliderFloat(const char *label, float *adr, float min, float max)
{
	return ImGui::SliderFloat(label, adr, min, max);
}

//=============================================================================
// スライダー処理
//=============================================================================
bool DebugSliderFloat(const std::string *label, float *adr, float min, float max)
{
	return ImGui::SliderFloat(label->c_str(), adr, min, max);
}

//=============================================================================
// カラーピッカー
//=============================================================================
void DebugColorEditor(const char *label, float array[4])
{
	ImGui::ColorEdit4(label, array);
}

//=============================================================================
// 改行
//=============================================================================
void DebugNewLine()
{
	ImGui::NewLine();
}

//=============================================================================
// 連続行
//=============================================================================
void DebugSameLine()
{
	ImGui::SameLine();
}

//=============================================================================
// ツリー構造展開
//=============================================================================
void DebugTreeExpantion(bool isOpen)
{
	ImGui::SetNextTreeNodeOpen(isOpen, ImGuiSetCond_Once);
}

//=============================================================================
// ツリーにプッシュ
//=============================================================================
bool DebugTreePush(const char *label)
{
	return ImGui::TreeNode(label);
}

//=============================================================================
// ツリーからポップ
//=============================================================================
void DebugTreePop()
{
	ImGui::TreePop();
}

//=============================================================================
// プログラスバー
//=============================================================================
void DebugProgressBar(float fraction, const char *label, D3DXVECTOR2 size)
{
	ImGui::ProgressBar(fraction, ImVec2(size.x, size.y), label);
}

//=============================================================================
// 3次元ベクトル入力
//=============================================================================
void DebugInputVector3(const char *label, D3DXVECTOR3 *vec)
{
	ImGui::InputFloat3(label, (float*)vec);
}

//=============================================================================
// 2次元ベクトル入力
//=============================================================================
void DebugInputVector2(const char *label, D3DXVECTOR2 *vec)
{
	ImGui::InputFloat2(label, (float*)vec);
}

//=============================================================================
// float型入力
//=============================================================================
void DebugInputFloat(const char *label, float *var)
{
	ImGui::InputFloat(label, var);
}
