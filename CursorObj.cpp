//=============================================================================
//
// キャラクターセレクトカーソル [CursorObj.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "CursorObj.h"
#include "Player.h"
#include "Input.h"
#include "_2dobj.h"
#include "MyLibrary.h"
#include "Sound.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CURSOROBJ_SIZE		D3DXVECTOR3(114.0f, 114.0f, 0.0f)	// テクスチャサイズ
#define CURSOROBJ_POS		D3DXVECTOR3(350.0f, 100.0f, 0.0f)
#define CURSOROBJ_SPACE		D3DXVECTOR3(150.0f, 150.0f, 0.0f)	// 表示間隔

//*****************************************************************************
// メンバ変数の初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9	CursorObj::D3DTexture = NULL;				// テクスチャのポインタ
LPDIRECT3DTEXTURE9	CursorObj::D3DTexture2 = NULL;				// テクスチャのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CursorObj::CursorObj(int playerNo, int cursorNo)
{
	ResourceManager::Instance()->GetTexture("SelectCursor", &D3DTexture);

	this->playerNo = playerNo;
	this->cursorNo = cursorNo;
	this->selectNo = cursorNo;
	use = true;
	pos.x = cursorNo * CURSOROBJ_SPACE.x + CURSOROBJ_POS.x;
	pos.y = playerNo * CURSOROBJ_SPACE.y + CURSOROBJ_POS.y;
	pos.z = 0.0f;
	
	// 頂点情報の作成
	MakeVertex();
	MakeVert();
}

//=============================================================================
// デストラクタ
//=============================================================================
CursorObj::~CursorObj()
{
	// リソースの開放はリソースマネージャに任せるので、参照をやめるだけ
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void CursorObj::Update()
{
	if (use == true)
	{
		// 移動
		Move();

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CursorObj::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャ座標をセット
	SetTexture();
	SetVertex();

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}
void CursorObj::Draw2()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャ座標をセット
	SetTex();
	SetVert();

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture2);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT CursorObj::MakeVertex(void)
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
	SetTexture();

	return S_OK;
}
HRESULT CursorObj::MakeVert(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVert();

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
void CursorObj::SetTexture()
{
	int x = this->selectNo;
	float sizeX = 1.0f / (float)CURSOROBJ_DIVIDE_X;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 0.5f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.5f);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CursorObj::SetTex()
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void CursorObj::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + CURSOROBJ_SIZE.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + CURSOROBJ_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + CURSOROBJ_SIZE.x, pos.y + CURSOROBJ_SIZE.y, pos.z);
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void CursorObj::SetVert()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f + CURSOROBJ_SIZE.x*5, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f, pos.y + CURSOROBJ_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f + CURSOROBJ_SIZE.x*5, pos.y + CURSOROBJ_SIZE.y, pos.z);
}

//=============================================================================
// 移動
//=============================================================================
void CursorObj::Move()
{
	// 右に移動
	if (GetKeyboardRepeat(DIK_D) || IsButtonRepeated(this->playerNo, STICK_RIGHT))
	{
		PlaySound(SE_SELECT);
		this->selectNo = LoopCountDown(this->selectNo, 0, CURSOROBJ_MAX - 1);
	}

	// 左に移動
	if (GetKeyboardRepeat(DIK_A) || IsButtonRepeated(this->playerNo, STICK_LEFT))
	{
		PlaySound(SE_SELECT);
		this->selectNo = LoopCountUp(this->selectNo, 0, CURSOROBJ_MAX - 1);
	}
}