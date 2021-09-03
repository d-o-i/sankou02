//=============================================================================
//
// ステージ名表示 [StageName.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageName.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGENAME_POS	D3DXVECTOR3(SCREEN_CENTER_X / 2, SCREEN_CENTER_Y + SCREEN_CENTER_Y / 2, 0.0f)
#define STAGENAME_SIZE	D3DXVECTOR3(SCREEN_WIDTH / 2.5f, SCREEN_HEIGHT / 2.5f, 0.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
StageName::StageName()
{
	ResourceManager::Instance()->GetTexture("StageName", &D3DTexture);

	pos = STAGENAME_POS;
	size = STAGENAME_SIZE;
	PatternAnim = 1;
	use = true;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
StageName::~StageName()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void StageName::Update()
{

}

//=============================================================================
// 描画
//=============================================================================
void StageName::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// 頂点座標の作成
//=============================================================================
void StageName::MakeVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}