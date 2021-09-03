//=============================================================================
//
// キャラクターの顔表示 [Face.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Face.h"
#include "CursorObj.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FACE_SIZE		D3DXVECTOR3(75.0f, 75.0f, 0.0f)		// テクスチャサイズ
//#define FACE_SIZE		D3DXVECTOR3(50.0f, 50.0f, 0.0f)		// テクスチャサイズ
#define FACE_POS		D3DXVECTOR3(17.0f, 19.0f, 0.0f)
//#define FACE_POS		D3DXVECTOR3(31.0f, 30.5f, 0.0f)
#define FACE_SPACE		(318.0f)							// 表示間隔

//=============================================================================
// コンストラクタ
//=============================================================================
Face::Face(int playerNo, int charNo)
{
	ResourceManager::Instance()->GetTexture("SelectCursor", &D3DTexture);

	this->playerNo = playerNo;
	this->charNo = charNo;

	use = true;
	pos = FACE_POS;
	pos.x += this->playerNo * FACE_SPACE;

	// 頂点情報の作成
	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Face::~Face()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void Face::Update()
{

}

//=============================================================================
// 描画処理
//=============================================================================
void Face::Draw()
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
// 頂点の作成
//=============================================================================
HRESULT Face::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertex();

	// rhwの設定
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	SetTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void Face::SetTexture()
{
	int x = this->charNo;
	float sizeX = 1.0f / (float)CURSOROBJ_DIVIDE_X;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.51f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.51f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void Face::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + FACE_SIZE.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + FACE_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + FACE_SIZE.x, pos.y + FACE_SIZE.y, pos.z);
}