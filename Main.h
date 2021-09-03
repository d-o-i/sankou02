//=============================================================================
//
// メイン処理 [Main.h]
// Author :  
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include <time.h>
#include <dshow.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <map>

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"

// ImGui関係
#include "imgui\imgui.h"
#include "imgui\imconfig.h"
#include "imgui\imgui_internal.h"
#include "imgui\imstb_rectpack.h"
#include "imgui\imstb_textedit.h"
#include "imgui\imstb_truetype.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"

#include "Struct.h"
#include "MyColor.h"

//*****************************************************************************
// 名前空間の確保
//*****************************************************************************
using namespace std;

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#define SCREEN_WIDTH	(1280)											// ウインドウの幅
#define SCREEN_HEIGHT	(720)											// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)								// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)								// ウインドウの中心Ｙ座標

#define TIME_PER_FRAME	(1/60.0f)										// 1/60秒

#define	NUM_VERTEX		(4)												// 頂点数
#define	NUM_POLYGON		(2)												// ポリゴン数

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(5000.0f)										// ビュー平面のFarZ値


// デストラクタ
#define SAFE_FREE(object)			{if(object){free (object);			(object) = NULL;}}
#define SAFE_DELETE(object)			{if(object){delete(object);			object = NULL;}}
#define SAFE_DELETE_ARRAY(object)	{if(object){delete[](object);		object = NULL;}}
#define SAFE_RELEASE(object)		{if(object){(object)->Release();	(object) = NULL;}}

#define GetMonitorRect(rc) SystemParametersInfo(SPI_GETWORKAREA, 0, rc, 0)	// モニター矩形

// ベクトルメモリ解放
template <class T>
void ReleaseVector(vector<T>& vt)
{
	vector<T> vtTemp;
	vtTemp.swap(vt);
}

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice();	// デバイスを取得する

#endif