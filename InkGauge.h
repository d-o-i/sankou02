//=============================================================================
//
// インクゲージ表示処理 [InkGauge.h]
// Author :  
//
//=============================================================================
#ifndef _InkGauge_H_
#define _InkGauge_H_

#include "_2dobj.h"

// インクの種類
enum InkType
{
	ColorInk,
	BlackInk,
	InkNum,		// インクの最大数
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class InkGauge : public _2dobj
{
private:
	int			InkType = 0;					// 使用するインクの種類(0=color,1=black)
	int			Owner = 0;						// 所有者
	Vertex2D	GaugeVertex[NUM_VERTEX];		// 頂点情報格納構造体
	Vertex2D	FrameVertex[NUM_VERTEX];		// 頂点情報格納構造体

	LPDIRECT3DTEXTURE9 Texture_InkGauge;
	LPDIRECT3DTEXTURE9 Texture_GaugeFrame;

	HRESULT MakeVertex_GaugeFrame(int InkValue);
	HRESULT MakeVertex_InkGauge(int InkValue);
	void SetTexture_GaugeFrame(int InkValue);
	void SetTexture_InkGauge(int InkValue);
	void SetVertex_GaugeFrame(int InkValue);
	void SetVertex_InkGauge(int InkValue);

public:
	InkGauge(int InkType, int PlayerNo);
	~InkGauge();

	// オーバーライド関数
	void Update(int InkValue);
	void Draw();
};

#endif
