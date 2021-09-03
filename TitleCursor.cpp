//=============================================================================
//
// タイトル画面処理 [TitleCursor.cpp]
// Author :  
//
//=============================================================================
#include "TitleCursor.h"
#include "Main.h"
#include "ResourceManager.h"
#include "SceneTitle.h"
#include "Input.h"
#include "Player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_CURSOR_SIZE			D3DXVECTOR3(400.0f * 0.75f, SCREEN_CENTER_Y*0.50f, 0.0f)// テクスチャサイズ
#define TITLE_CURSOR_POS_START		D3DXVECTOR3(400.0f, SCREEN_CENTER_Y * 1.50f, 0.0f)		// テクスチャ座標
#define TITLE_CURSOR_POS_EXIT		D3DXVECTOR3(250.0f, SCREEN_CENTER_Y * 1.55f, 0.0f)		// テクスチャ座標

//=============================================================================
// コンストラクタ
//=============================================================================
TITLECURSOR::TITLECURSOR()
{
	ResourceManager::Instance()->GetTexture("TitleCursor", &D3DTexture);
	size = TITLE_CURSOR_SIZE;
	pos = TITLE_CURSOR_POS_START;

	use = true;
	PatternAnim = 1;

	// 頂点情報の作成
	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
TITLECURSOR::~TITLECURSOR()
{
	// リソースの開放はリソースマネージャに任せるため、参照をやめるだけ
	D3DTexture = NULL;

}
//=============================================================================
// 更新処理
//=============================================================================
void  TITLECURSOR::Update()
{

	if (use == true)
	{
		// テクスチャ座標をセット
		SetTexture(PatternAnim);

		for (int playerNo = 0; playerNo < PLAYER_MAX; playerNo++)
		{
			if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(playerNo, STICK_UP))
			{

				SetVertex();

			}
			if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(playerNo, STICK_DOWN))
			{
				SetVertexMove(TITLE_CURSOR_POS_EXIT);
			}

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void TITLECURSOR::Draw()
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
HRESULT TITLECURSOR::MakeVertex(void)
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
void TITLECURSOR::SetTexture(int cntPattern)
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
void TITLECURSOR::SetVertex(void)
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
void TITLECURSOR::SetVertexMove(D3DXVECTOR3 pos)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);
}
