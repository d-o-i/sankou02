//=============================================================================
//
// インクゲージ表示処理 [Ink.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "InkGauge.h"
#include "PaintManager.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INKGAUGE_DIVIDE_X		(1)
#define INKGAUGE_DIVIDE_Y		(5)

#define InkGauge_Size			D3DXVECTOR2(120.0f, 30.0f)	// 表示サイズ
#define ColorInkGauge_Pos		D3DXVECTOR3(86.0f, 35.0f, 0.0f)
#define BlackInkGauge_Pos		D3DXVECTOR3(86.0f, 45.0f, 0.0f)
#define InkGauge_Interval		(318.0f)

#define GaugeFrame_Size			D3DXVECTOR3(200.0f, 88.0f, 0.0f)		// テクスチャサイズ
#define GaugeFrame_Pos			D3DXVECTOR3(10.0f, 10.0f, 0.0f)
#define GaugeFrame_Interval		(318.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
InkGauge::InkGauge(int InkType, int PlayerNo)
{
	// インクの初期化
	use = true;
	this->InkType = InkType;
	this->Owner = PlayerNo;

	if (this->InkType == ColorInk)
	{
		ResourceManager::Instance()->GetTexture("ColorInkFrame", &Texture_GaugeFrame);
	}
	else if (this->InkType == BlackInk)
	{
		ResourceManager::Instance()->GetTexture("BlackInkFrame", &Texture_GaugeFrame);
	}

	ResourceManager::Instance()->GetTexture("InkGauge", &Texture_InkGauge);

	// インクの頂点情報
	MakeVertex_GaugeFrame(INK_MAX);
	// インク用フレームの頂点情報
	MakeVertex_InkGauge(INK_MAX);
}

//=============================================================================
// デストラクタ
//=============================================================================
InkGauge::~InkGauge()
{
	// テクスチャの開放
	Texture_InkGauge = NULL;
	Texture_GaugeFrame = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void InkGauge::Update(int InkValue)
{
	if (use == true)
	{
		// 頂点座標の設定
		SetVertex_InkGauge(InkValue);
		//テクスチャ座標をセット
		SetTexture_InkGauge(InkValue);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void InkGauge::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (use)
	{
		Device->SetTexture(0, Texture_InkGauge);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GaugeVertex, sizeof(Vertex2D));

		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		Device->SetTexture(0, Texture_GaugeFrame);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, FrameVertex, sizeof(Vertex2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT InkGauge::MakeVertex_GaugeFrame(int InkValue)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定
	SetVertex_GaugeFrame(InkValue);

	// rhwの設定
	FrameVertex[0].rhw = 1.0f;
	FrameVertex[1].rhw = 1.0f;
	FrameVertex[2].rhw = 1.0f;
	FrameVertex[3].rhw = 1.0f;

	// 反射光の設定
	FrameVertex[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexture_GaugeFrame(InkValue);

	return S_OK;
}

HRESULT InkGauge::MakeVertex_InkGauge(int InkValue)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定
	SetVertex_InkGauge(InkValue);

	// rhwの設定
	GaugeVertex[0].rhw = 1.0f;
	GaugeVertex[1].rhw = 1.0f;
	GaugeVertex[2].rhw = 1.0f;
	GaugeVertex[3].rhw = 1.0f;

	// 反射光の設定
	GaugeVertex[0].rhw = 1.0f;
	GaugeVertex[1].rhw = 1.0f;
	GaugeVertex[2].rhw = 1.0f;
	GaugeVertex[3].rhw = 1.0f;

	// テクスチャ座標の設定
	SetTexture_InkGauge(InkValue);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void InkGauge::SetTexture_GaugeFrame(int InkValue)
{
	// テクスチャ座標の設定
	FrameVertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	FrameVertex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	FrameVertex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	FrameVertex[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

void InkGauge::SetTexture_InkGauge(int InkValue)
{
	int y = 0;
	if (InkType == BlackInk)
	{
		y = BlackInkColor;
	}
	else if (InkType == ColorInk)
	{
		y = Owner;
	}
	float sizeY = 1.0f / INKGAUGE_DIVIDE_Y;
	float InkPercent = (float)InkValue / INK_MAX;

	// テクスチャ座標の設定
	GaugeVertex[0].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY);
	GaugeVertex[1].tex = D3DXVECTOR2(1.0f * InkPercent, (float)(y)* sizeY);
	GaugeVertex[2].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY + sizeY);
	GaugeVertex[3].tex = D3DXVECTOR2(1.0f * InkPercent, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void InkGauge::SetVertex_GaugeFrame(int InkValue)
{
	D3DXVECTOR3 pos = GaugeFrame_Pos + D3DXVECTOR3(this->Owner * GaugeFrame_Interval, 0.0f, 0.0f);

	// 頂点座標の設定
	FrameVertex[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	FrameVertex[1].vtx = D3DXVECTOR3(pos.x + GaugeFrame_Size.x, pos.y, 0.0f);
	FrameVertex[2].vtx = D3DXVECTOR3(pos.x, pos.y + GaugeFrame_Size.y, 0.0f);
	FrameVertex[3].vtx = D3DXVECTOR3(pos.x + GaugeFrame_Size.x, pos.y + GaugeFrame_Size.y, 0.0f);
}

void InkGauge::SetVertex_InkGauge(int InkValue)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float InkPercent = (float)InkValue / INK_MAX;

	if (this->InkType == ColorInk)
	{
		pos = ColorInkGauge_Pos + D3DXVECTOR3(this->Owner * InkGauge_Interval, 0.0f, 0.0f);
	}
	else if (this->InkType == BlackInk)
	{
		pos = BlackInkGauge_Pos + D3DXVECTOR3(this->Owner * InkGauge_Interval, 0.0f, 0.0f);
	}

	// 頂点座標の設定
	GaugeVertex[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	GaugeVertex[1].vtx = D3DXVECTOR3(pos.x + InkGauge_Size.x * InkPercent, pos.y, 0.0f);
	GaugeVertex[2].vtx = D3DXVECTOR3(pos.x, pos.y + InkGauge_Size.y, 0.0f);
	GaugeVertex[3].vtx = D3DXVECTOR3(pos.x + InkGauge_Size.x * InkPercent, pos.y + InkGauge_Size.y, 0.0f);
}


