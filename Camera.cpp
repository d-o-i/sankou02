//=============================================================================
//
// カメラ処理 [Camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Camera.h"
#include "Input.h"
#include "SceneManager.h"
#include "DebugWindow.h"
#include "MyLibrary.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAMERA_POS		D3DXVECTOR3(0.0f, 10.0f, -500.0f)				// カメラの初期位置
#define CAMERA_POS_RESULT	D3DXVECTOR3(0.0f, -100.0f, -450.0f)			// リザルト画面でのカメラの初期位置
#define CAMERA_AT		D3DXVECTOR3(0.0f, 0.0f, 0.0f)					// カメラの注視点
#define CAMERA_AT_RESULT	D3DXVECTOR3(0.0f, 300.0f, 0.0f)
#define CAMERA_AT_TITLE		D3DXVECTOR3(100.0f, -100.0f, 0.0f)

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.002f)							// カメラの回転量

#define	INTERVAL_CAMERA_R	(12.5f)										// モデルの視線の先までの距離
#define	RATE_CHASE_CAMERA_P	(0.35f)										// カメラの視点への補正係数
#define	RATE_CHASE_CAMERA_R	(0.20f)										// カメラの注視点への補正係数
#define CAMERA_SPD			(0.05f)										// 古いカメラ注視点と新しいカメラ注視点に差がある場合にカメラ位置を更新するスピード

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA cameraWk;

void DebugCamera();

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	cameraWk.pos = CAMERA_POS;
	cameraWk.at = CAMERA_AT;
	if (GetScene() == nSceneResult)
	{
		cameraWk.at = CAMERA_AT_RESULT;
		cameraWk.pos = cameraWk.at + CAMERA_POS_RESULT;
	}
	else if (GetScene() == nSceneTitle)
	{
		cameraWk.at = CAMERA_AT_TITLE;
		cameraWk.pos = cameraWk.at + CAMERA_POS;
	}
	cameraWk.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraWk.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	float vx, vz;
	vx = cameraWk.pos.x - cameraWk.at.x;
	vz = cameraWk.pos.z - cameraWk.at.z;
	cameraWk.distance = sqrtf(vx * vx + vz * vz);
}

//=============================================================================
// カメラの更新処理（SceneGame用）
//=============================================================================
void UpdateCamera(D3DXVECTOR3 _at)
{
	// 一番前にいるキャラクタの座標を新しい注視点とする
	D3DXVECTOR3 newAt = _at;

	// 更新前の注視点と新しい注視点の差をベクトルにする
	D3DXVECTOR3 DistVec = newAt - cameraWk.at;

	// 徐々に新しい注視点に近づける
	cameraWk.at += DistVec * CAMERA_SPD;
	
	// 座標は注視点に対して平行移動する
	cameraWk.pos = cameraWk.at + CAMERA_POS;

}

//=============================================================================
// カメラの更新処理（SceneResult用）
//=============================================================================
void UpdateCamera()
{
	static float x = cameraWk.pos.x, y = cameraWk.pos.y, z = cameraWk.pos.z;

	// 左回転させつづける
	cameraWk.rot.y += VALUE_ROTATE_CAMERA;
	if (cameraWk.rot.y > D3DX_PI)
	{
		cameraWk.rot.y = -D3DX_PI;
	}
	x = cameraWk.distance * sinf(cameraWk.rot.y);
	z = cameraWk.distance * cosf(cameraWk.rot.y);

#ifndef _DEBUG_
	// カメラのY軸回転
	if (GetKeyboardPress(DIK_RIGHT))
	{
		cameraWk.rot.y -= VALUE_ROTATE_CAMERA;
		if (cameraWk.rot.y < -D3DX_PI)
		{
			cameraWk.rot.y = D3DX_PI;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		cameraWk.rot.y += VALUE_ROTATE_CAMERA;
		if (cameraWk.rot.y > D3DX_PI)
		{
			cameraWk.rot.y = -D3DX_PI;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}

	// カメラの注視点と視点の距離変更
	if (GetKeyboardPress(DIK_UP))
	{
		if (cameraWk.distance > 5.0f)
		{
			cameraWk.distance -= 5.0f;
		}
		else if (cameraWk.distance == 5.0f)
		{
			cameraWk.distance = 1.0f;
		}
		else if (cameraWk.distance < 5.0f)
		{
			cameraWk.distance = 1.0f;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		cameraWk.distance += 5.0f;
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}

	//DebugCamera();
#endif

	cameraWk.pos = cameraWk.at + D3DXVECTOR3(x, y, z);	// カメラの位置（視点）
	cameraWk.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの注視点＝ワールドの中心点
	cameraWk.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 3D空間の上方向はどちら？＝Yが＋方向が上

}

//=============================================================================
// タイトル画面でのカメラの更新
//=============================================================================
void UpdateTitleCamera()
{
	if (cameraWk.at.x < 9800.0f)
	{
		cameraWk.at.x += 1.0f;
		cameraWk.at.y -= 0.1f;
	}
	cameraWk.pos = cameraWk.at + CAMERA_POS;
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&cameraWk.mtxView,
		&cameraWk.pos,		// カメラの視点
		&cameraWk.at,		// カメラの注視点
		&cameraWk.up);		// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &cameraWk.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk.mtxProj);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&cameraWk.mtxProj,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &cameraWk.mtxProj);

}

//=============================================================================
//カメラの情報を取得する
//=============================================================================
CAMERA *GetCamera(void)
{
	return &cameraWk;
}

void DebugCamera()
{
	BeginDebugWindow("Camera");

	DebugText("Pos X:%f Y:%f Z:%f", cameraWk.pos.x, cameraWk.pos.y, cameraWk.pos.z);
	DebugText("At X:%f Y:%f Z:%f", cameraWk.at.x, cameraWk.at.y, cameraWk.at.z);
	DebugText("Rot X:%f Y:%f Z:%f", cameraWk.rot.x, cameraWk.rot.y, cameraWk.rot.z);

	EndDebugWindow("Camera");
}