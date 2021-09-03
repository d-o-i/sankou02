//=============================================================================
//
// リザルト画面 [Result.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Result.h"
#include "carslobj.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_SIZE01			D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f)	// テクスチャサイズ
#define RESULT_POS01			D3DXVECTOR3(0.0f,0.0f,0.0f)						// テクスチャ座標

//=============================================================================
// コンストラクタ
//=============================================================================
RESULT::RESULT()
{
	ResourceManager::Instance()->GetTexture("Result", &D3DTexture);

	use = true;
	pos = RESULT_POS01;
	size = RESULT_SIZE01;

	PatternAnim = 1;

	// 頂点情報の作成
	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
RESULT::~RESULT()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void  RESULT::Update()
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
void RESULT::Draw()
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
HRESULT RESULT::MakeVertex(void)
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
void RESULT::SetTexture(int cntPattern)
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
void RESULT::SetVertex(void)
{

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);
}
