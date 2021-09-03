//=============================================================================
//
// タイトル画面処理 [Title.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Title.h"
#include "CursorObj.h"
#include "Sound.h"
#include "Input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneTitle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_SIZE			D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.75f, 0.0f)	// テクスチャサイズ
#define LOGO_POS			D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.75f, 0.0f)		// テクスチャ座標

#define MENU_SIZE			D3DXVECTOR3(702.0f * 0.45f, 515.0f * 0.45f, 0.0f)				// テクスチャサイズ
#define MENU_POS			D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.65f, 0.0f)		// テクスチャ座標
//=============================================================================
// コンストラクタ
//=============================================================================
TITLE::TITLE(int num)
{
	switch (num)
	{
	case TitleLogo:
		ResourceManager::Instance()->GetTexture("TitleLogo", &D3DTexture);
		size = LOGO_SIZE;
		pos = LOGO_POS;
		break;
	case TitleMenu:
		ResourceManager::Instance()->GetTexture("TitleMenu", &D3DTexture);
		size = MENU_SIZE;
		pos = MENU_POS;
		break;

	default:
		break;
	}

	use = true;
	PatternAnim = 1;

	// 頂点情報の作成
	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
TITLE::~TITLE()
{
	// リソースの開放はリソースマネージャに任せるため、参照をやめるだけ
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void  TITLE::Update()
{

	if (use == true)
	{

		// テクスチャ座標をセット
		SetTexture(PatternAnim);

		SetVertex();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void TITLE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	if (use == true)
	{
		// テクスチャの設定（ポリゴンの描画前に読み込んだテクスチャのセットを行う）
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何も貼らないことを指定するpDevice->SetTexture(0,NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT TITLE::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の設定
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
// テクスチャ座標の設定
//=============================================================================
void TITLE::SetTexture(int cntPattern)
{
	int x = cntPattern;
	int y = cntPattern;
	float sizeX = 1.0f;
	float sizeY = 1.0f;


	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void TITLE::SetVertex(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// 頂点座標の移動
//=============================================================================
void TITLE::SetVertexMove(D3DXVECTOR3 pos)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);
}
