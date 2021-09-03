//=============================================================================
//
// キャラクターセレクトフレーム [CharSelectFrame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "CharSelectFrame.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ImageSize		D3DXVECTOR3(150.0f, 150.0f, 0.0f)		// テクスチャサイズ

//=============================================================================
// コンストラクタ
//=============================================================================
CharSelectFrame::CharSelectFrame(D3DXVECTOR2 Pos)
{
	ResourceManager::Instance()->GetTexture("CharSelectFrame", &D3DTexture);

	this->use = false;
	this->pos = (D3DXVECTOR3)Pos;

	// 頂点情報の作成
	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
CharSelectFrame::~CharSelectFrame()
{
	// リソースの開放はリソースマネージャに任せるので、参照をやめるだけ
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void CharSelectFrame::Update()
{
	if (use == true)
	{
		if (AlphaPlus)
		{
			Alpha += 5;
			if (Alpha >= 192)
			{
				Alpha = 192;
				AlphaPlus = false;
			}
		}
		else
		{
			Alpha -= 5;
			if (Alpha <= 0)
			{
				Alpha = 0;
				AlphaPlus = true;
			}
		}

		SetAlpha(Alpha);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CharSelectFrame::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (use == true)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT CharSelectFrame::MakeVertex(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - ImageSize.x / 2, pos.y - ImageSize.y / 2, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + ImageSize.x / 2, pos.y - ImageSize.y / 2, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - ImageSize.x / 2, pos.y + ImageSize.y / 2, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + ImageSize.x / 2, pos.y + ImageSize.y / 2, 0.0f);

	// rhwの設定
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// アルファ値の設定
//=============================================================================
void CharSelectFrame::SetAlpha(int Alpha)
{
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
}
