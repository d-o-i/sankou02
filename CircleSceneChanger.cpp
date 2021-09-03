//=============================================================================
//
// 円形シーン切り替え [CircleSceneChanger.cpp]
// Author : 
// 
//=============================================================================
#include "Main.h"
#include "CircleSceneChanger.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHANGE_TIME	(60)

//=============================================================================
// 更新
//=============================================================================
void CircleSceneChanger::Update()
{
	if (!useMask)
		return;

	cntFrame++;

	// スケールを小さくする
	scl -= D3DXVECTOR3(1.0f / (float)CHANGE_TIME, 1.0f / (float)CHANGE_TIME, 0.0f);

	SetVertexMask();

	// 終了確認
	if (cntFrame == CHANGE_TIME)
	{
		useMask = false;

		if (isChangerOut)
			useChanger = false;

		// コールバックの呼び出し
		if (callback != NULL)
			callback();

		callback = NULL;
	}
}

//=============================================================================
// 開始
//=============================================================================
void CircleSceneChanger::SetChanger(bool isStart, std::function<void(void)> func)
{
	// すでにマスク中であれば開始しない
	if (useMask)
		return;

	// 同じ遷移状態の場合開始しない
	if (isStart == useChanger)
		return;

	// パラメータ初期化
	scl = isStart ? D3DXVECTOR3(1.0f, 1.0f, 0.0f) : D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	cntFrame = 0;

	// コールバック登録
	callback = func;

	// フラグ初期化
	useMask = true;
	useChanger = true;
	isChangerOut = !isStart;
}
