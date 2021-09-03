//=============================================================================
//
// ミニプレイヤー処理 [MiniPlayer.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MiniPlayer.h"
#include "Map.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MiniPlayer_Divide_X		(4)									// 横分割
#define MiniPlayer_Size_X		(15)
#define MiniPlayer_Size_Y		(30)
#define MiniPlayer_Pos_X		(10)
#define MiniPlayer_Pos_Y		(80)
#define Distance_Screen			(1200.0f)
#define Distance_World			(GOAL_POS.x - START_POS.x)

//=============================================================================
// コンストラクタ
//=============================================================================
MiniPlayer::MiniPlayer(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("MiniPlayer", &D3DTexture);

	pos = D3DXVECTOR3(MiniPlayer_Pos_X, MiniPlayer_Pos_Y, 0.0f);

	// 頂点情報の作成
	MakeVertex(PlayerNo);
}
//=============================================================================
// デストラクタ
//=============================================================================
MiniPlayer::~MiniPlayer()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void MiniPlayer::Update(D3DXVECTOR3 PlayerPos)
{
	pos.x = MiniPlayer_Pos_X + Distance_Screen * (PlayerPos.x / Distance_World);

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// 描画
//=============================================================================
void MiniPlayer::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);

	Device->SetTexture(0, D3DTexture);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}
//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MiniPlayer::MakeVertex(int PlayerNo)
{
	// 頂点座標の設定
	SetVertex();

	// テクスチャのパースペクティブコレクト用
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexture(PlayerNo);

	return S_OK;

}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void MiniPlayer::SetTexture(int PlayerNo)
{
	float SizeX = 1.0f / MiniPlayer_Divide_X;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)PlayerNo* SizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)PlayerNo* SizeX + SizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)PlayerNo* SizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)PlayerNo* SizeX + SizeX, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void MiniPlayer::SetVertex(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + MiniPlayer_Size_X, pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + MiniPlayer_Size_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + MiniPlayer_Size_X, pos.y + MiniPlayer_Size_Y, 0.0f);
}