//=============================================================================
//
// フィニッシュUI [Finish.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Finish.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FINISH_SIZE		D3DXVECTOR3(1024.0f, 512.0f, 0.0f)
#define FINISH_MOVE_SPEED	(100.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
Finish::Finish()
{
	ResourceManager::Instance()->GetTexture("Finish", &D3DTexture);

	use = true;
	pos = D3DXVECTOR3(SCREEN_WIDTH + FINISH_SIZE.x / 2, SCREEN_CENTER_Y, 0.0f);
	PatternAnim = 1;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Finish::~Finish()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void Finish::Update()
{
	if(use)
	{
		// 右から真ん中に出てくる
		if (pos.x > SCREEN_CENTER_X)
		{
			pos.x -= FINISH_MOVE_SPEED;
		}
		else
		{			
			pos.x = SCREEN_CENTER_X;
		}

		SetVertex();
	}
}

//=============================================================================
// 描画
//=============================================================================
void Finish::Draw()
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
// 頂点情報の作成
//=============================================================================
HRESULT Finish::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertex();

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

	return S_OK;
}

//=============================================================================
// 頂点座標の作成
//=============================================================================
void Finish::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - FINISH_SIZE.x / 2, pos.y - FINISH_SIZE.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + FINISH_SIZE.x / 2, pos.y - FINISH_SIZE.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - FINISH_SIZE.x / 2, pos.y + FINISH_SIZE.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + FINISH_SIZE.x / 2, pos.y + FINISH_SIZE.y / 2, pos.z);
}