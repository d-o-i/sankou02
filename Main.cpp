//=============================================================================
//
// メイン処理 [Main.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "SceneManager.h"
#include "DebugWindow.h"
#include "Icon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"BrushRunner"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
bool SetWindowCenter(HWND hWnd);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
HWND hWnd;											// ウインドウハンドル
LPDIRECT3D9			g_pD3D = NULL;					// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
int					FPSCount;						// FPSカウンタ
bool ShowAnotherWindow = false;						// imGui用別ウインドウのフラグ
#endif

SceneManager *pSceneManager;						// シーン管理

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	// 警告
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;							// 最終実行時間
	DWORD dwFPSLastTime;							// 現在フレーム
	DWORD dwCurrentTime;							// 現在時刻
	DWORD dwFrameCount;								// フレームカウント

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// 構造体のサイズ
		CS_CLASSDC,									// ウインドウスタイル：CS_HREDRAW / CS_VREDRAW
		WndProc,									// ウインドウプロシージャのアドレス
		0,											// 予備メモリ
		0,											// ウインドウオブジェクト作成時に確保されるメモリサイズ
		hInstance,									// インスタンスハンドル
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1),	// アプリのショートカットなどで使用（アイコンの設定）
		LoadCursor(NULL, IDC_ARROW),				// ウインドウのクライアント上のマウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),					// ウインドウのクライアント領域の背景色
		NULL,										// メニュー名
		CLASS_NAME,									// ウインドウクラスの名前
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1)		// ウインドウのアイコン
	};
	MSG msg;										// ウインドウプロシージャに渡すメッセージ

													// ウィンドウクラスの登録
	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,																					// クラス名
		WINDOW_NAME,																				// ウインドウ名
		WS_OVERLAPPEDWINDOW,																		// ウインドウスタイル
		CW_USEDEFAULT,																				// ウインドウの左上座標（X）
		CW_USEDEFAULT,																				// ウインドウの左上座標（Y）
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,											// ウインドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),		// ウインドウ縦幅
		NULL,																						// オーナーウインドウのハンドル
		NULL,																						// メニューハンドル
		hInstance,																					// アプリケーションインスタンスのハンドル
		NULL);																						// ウインドウ作成データ

																									// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);

	// --------------------------------------  メッセージループ---------------------------------------------
	while (1)
	{
		int scene = GetScene();
		// ゲーム終了処理
		if (scene == nSceneExit)
		{
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)						//アプリケーションの終了要求
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// ゲーム処理
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				FPSCount = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	// -----------------------------------------------------------------------------------------------------

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 変数宣言
	static HANDLE Thread_Handle;

	switch (uMsg)
	{
		// 破棄
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// 非システムキーメッセージ
	case WM_KEYDOWN:
		switch (wParam)
		{
			// ESCキーが押された場合
		case VK_ESCAPE:
			DestroyWindow(hWnd);	// ウインドウを破棄する
			break;
		}
		break;
	default:
		break;
	}

	// デバッグウィンドウ
	DebugWindPrcHandler(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// ウインドウの表示位置を画面中央に設定
	SetWindowCenter(hWnd);

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

#ifdef _DEBUG
	InitDebugWindow(hWnd, g_pD3DDevice);
#endif
	pSceneManager = new SceneManager(hInstance, hWnd);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	UninitDebugWindow(0);
#endif

	delete pSceneManager;
	// デバイスの開放
	SAFE_RELEASE(g_pD3DDevice);

	// Direct3Dオブジェクトの開放
	SAFE_RELEASE(g_pD3D);

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	// 処理開始の時間を記録
	BeginTimerCount();

	UpdateDebugWindow();
#endif

	pSceneManager->Update();

#ifdef _DEBUG
	// 処理終了の時間を表示
	BeginDebugWindow("Information");

	ImGui::SetNextWindowPos(ImVec2(5, 600), ImGuiSetCond_Once);

	DebugText("UpdateTime = %.3f ms", GetProgressTimerCount());

	EndDebugWindow("Information");
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

#ifdef _DEBUG
	// 処理開始の時間を記録
	BeginTimerCount();
#endif

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		pSceneManager->Draw();

#ifdef _DEBUG
		// 処理終了の時間を表示
		BeginDebugWindow("Information");

		DebugText("DrawTime = %.3f ms\nFPS = %d\n", GetProgressTimerCount(), FPSCount);

		EndDebugWindow("Information");

		DrawDebugWindow();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=====================================================================================================
// ウインドウの表示位置を画面中央に
//=====================================================================================================
bool SetWindowCenter(HWND hWnd)
{
	// 変数宣言
	RECT WindowRegion;		//	ウィンドウ領域
	RECT DesktopRegion;	//	デスクトップ領域
	int WindowPositionX;	//	ウィンドウ位置 X
	int WindowPositionY;	//	ウィンドウ位置 Y
	int WindowSizeX;		//	ウィンドウサイズ X
	int WindowSizeY;		//	ウィンドウサイズ Y

							// 各サイズの取得
	GetMonitorRect(&DesktopRegion);				//	デスクトップのサイズを取得
	GetWindowRect(hWnd, &WindowRegion);	//	ウィンドウのサイズを取得

										// 各座標の割り出し
	WindowSizeX = (WindowRegion.right - WindowRegion.left);														//	ウインドウの横幅の割り出し
	WindowSizeY = (WindowRegion.bottom - WindowRegion.top);														//	ウインドウの縦幅の割り出し
	WindowPositionX = (((DesktopRegion.right - DesktopRegion.left) - WindowSizeX) / 2 + DesktopRegion.left);	//	横方向の中央座標軸の割り出し
	WindowPositionY = (((DesktopRegion.bottom - DesktopRegion.top) - WindowSizeY) / 2 + DesktopRegion.top);	//	縦方向の中央座標軸の割り出し

																											// ウィンドウを画面中央に移動
	return SetWindowPos	//	SetWindowPos関数：子ウィンドウ、ポップアップウィンドウ、またはトップレベルウィンドウのサイズ、位置、および Z オーダーを変更する
						//						これらのウィンドウは、その画面上での表示に従って順序が決められる、
						//						最前面にあるウィンドウは最も高いランクを与えられ、Zオーダーの先頭に置かれる
						(
							hWnd,											//	ウィンドウハンドル
							NULL,											//	配置順序のハンドル：先行するウィンドウのハンドルを指定
							WindowPositionX,								//	ウィンドウ左上隅の"X"座標を指定：横方向の位置 X
							WindowPositionY,								//	ウィンドウ左上隅の"Y"座標を指定：縦方向の位置 Y
							WindowSizeX,									//	ウィンドウの横幅を指定 X
							WindowSizeY,									//	ウィンドウの縦幅を指定 Y
							(SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER)	//	ウィンドウ位置のオプション：ウィンドウのサイズや、位置の変更に関するフラグを指定
						);

}
