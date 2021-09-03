//=============================================================================
//
// ポップアップ表示処理 [Pop.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Pop.h"
#include "Camera.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POP_WIDTH		(64.0f)							// 半径高さ
#define	POP_HEIGHT		(32.0f)							// 半径幅
#define POP_POS			D3DXVECTOR3(0.0f, 90.0f, -1.0f)	// 表示場所
#define POP_DIVIDE_X	(4)
#define POP_DIVIDE_Y	(1)
#define POP_ANIM_DIVIDE	(POP_DIVIDE_X * POP_DIVIDE_Y)

//=============================================================================
// コンストラクタ
//=============================================================================
Pop::Pop(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("Pop", &D3DTexture);

	this->PlayerNo = PlayerNo;
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	width = POP_WIDTH;
	height = POP_HEIGHT;

	// 頂点情報の作成
	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Pop::~Pop()
{
	// テクスチャの参照をやめる
	D3DTexture = NULL;

	// 頂点バッファの開放
	SAFE_RELEASE(D3DVtxBuff);
}

//=============================================================================
// 更新処理
//=============================================================================
void Pop::Update(D3DXVECTOR3 PlayerPos)
{
	pos = PlayerPos + POP_POS;
}

//=============================================================================
// 描画処理
//=============================================================================
void Pop::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX WorldMtx, TransMtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// 移動を反映
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &WorldMtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTexture);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT Pop::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,				// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, POP_HEIGHT / 2.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, POP_HEIGHT / 2.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, -POP_HEIGHT / 2.0f, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, -POP_HEIGHT / 2.0f, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	int x = this->PlayerNo;
	float sizeX = 1.0f / (float)POP_DIVIDE_X;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;
}