//=============================================================================
//
// ペイント [Paint.h]
// Author :  
//
//=============================================================================
#ifndef _PAINT_H_
#define _PAINT_H_

#include "Billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PAINT_WIDTH			(30.0f)						// 幅
#define	PAINT_HEIGHT		(30.0f)						// 高さ
#define PAINT_DIVIDE_X		(5)
#define PAINT_DIVIDE_Y		(1)
#define PAINT_DIVIDE		(PAINT_DIVIDE_X * PAINT_DIVIDE_Y)
#define DRAW_FRAME_COLOR	(300)						// 表示しておくフレーム数
#define DRAW_FRAME_BLACK	(60)						// 表示しておくフレーム数

enum InkColor
{
	RedInkColor,
	BlueInkColor,
	YellowInkColor,
	GreenInkColor,
	BlackInkColor
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Paint : public Billboard
{
private:
	bool					Use;			// 使用しているかどうか
	bool					InScreen;		// 画面内フラグ
	int						time;			// 表示する時間
	int						NodeID;			// 四分木用ノードID
	int						Owner;			// 所有者
	int						PaintColor;		// ペイントカラー
	D3DXVECTOR2				ScreenPos;		// スクリーン座標

	static float			HalfSize;		// ペイントサイズの半分

	void SetColor();
	HRESULT MakeVertex();
	// ワールド座標からスクリーン座標に変換する
	void CheckInScreen(D3DXMATRIX WorldMatrix);

public:
	Paint(int Owner, int PaintColor);
	~Paint();

	void Update();
	void Draw();

	// ゲッター
	bool GetUse() { return this->Use; };
	bool GetInScreen() { return this->InScreen; };
	D3DXVECTOR3 GetPos() { return pos; };
	int GetNodeID(void) { return this->NodeID; };
	int GetOwner(void) { return this->Owner; };
	int GetPaintColor(void) { return this->PaintColor; };
	D3DXVECTOR2 GetScreenPos(void) { return this->ScreenPos; };
	static float GetPaintRadius(void) { return Paint::HalfSize; };

	// セッター
	void SetPos(D3DXVECTOR3 _pos) { pos = _pos; };
	void SetUse(bool Flag) { this->Use = Flag; };
	void SetTime(int _time) { time = _time; };
	void SetNodeID(int NodeID) { this->NodeID = NodeID; };
	void SetScreenPos(D3DXVECTOR2 ScreenPos) { this->ScreenPos = ScreenPos; };
};

#endif
