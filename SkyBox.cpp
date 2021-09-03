//=============================================================================
//
// 空処理 [SkyBox.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "SkyBox.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SKYBOX_DIVIDE_X			(4)									// テクスチャ分割数
#define SKYBOX_DIVIDE_Y			(3)
#define SURFACE_NUM				(6)									// 面の数
#define SKYBOX_LENGTH			(3000.0f)							// 大きさ
#define SKYBOX(Alpha)			D3DCOLOR_RGBA(135, 206, 235, Alpha)	// 空の色

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
SkyBox::SkyBox()
{
	ResourceManager::Instance()->GetTexture("SkyBox", &D3DTexture);

	// 位置・回転・スケールの初期設定
	pos = D3DXVECTOR3(0.0f, -500.0f, 0.0f);;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// 頂点情報の作成
	MakeVertex();

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
SkyBox::~SkyBox()
{
	SAFE_RELEASE(D3DVtxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void SkyBox::Update()
{
	// ちょっとずつ回転
	rot.y += 0.0001f;

}

//=============================================================================
// 描画処理
//=============================================================================
void SkyBox::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX WorldMtx, SclMtx, RotMtx, TransMtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// スケールを反映
	D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&RotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &RotMtx);

	// 移動を反映
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, D3DTexture);

	// ポリゴンの描画
	for (int i = 0; i < SURFACE_NUM; i++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT SkyBox::MakeVertex()
{
	int i = 0;
	int x = 0, y = 0;
	float sizeX = 1.0f / SKYBOX_DIVIDE_X;
	float sizeY = 1.0f / SKYBOX_DIVIDE_Y;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX * SURFACE_NUM,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定

	// 前
	pVtx[0].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[1].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[2].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[3].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	// 後ろ
	pVtx[4].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[5].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[6].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[7].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	// 上
	pVtx[8].vtx = pVtx[5].vtx;
	pVtx[9].vtx = pVtx[4].vtx;
	pVtx[10].vtx = pVtx[0].vtx;
	pVtx[11].vtx = pVtx[1].vtx;
	// 右
	pVtx[12].vtx = pVtx[1].vtx;
	pVtx[13].vtx = pVtx[4].vtx;
	pVtx[14].vtx = pVtx[3].vtx;
	pVtx[15].vtx = pVtx[6].vtx;
	// 底
	pVtx[16].vtx = pVtx[2].vtx;
	pVtx[17].vtx = pVtx[3].vtx;
	pVtx[18].vtx = pVtx[7].vtx;
	pVtx[19].vtx = pVtx[6].vtx;
	// 左
	pVtx[20].vtx = pVtx[5].vtx;
	pVtx[21].vtx = pVtx[0].vtx;
	pVtx[22].vtx = pVtx[7].vtx;
	pVtx[23].vtx = pVtx[2].vtx;


	// 法線ベクトルの設定
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// 反射光の設定
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定

	// 前
	x = 1;
	y = 1;
	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 後ろ
	x = 3;
	y = 1;
	pVtx[4].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[5].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[6].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[7].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 上
	x = 1;
	y = 0;
	pVtx[8].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[9].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[10].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[11].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 右
	x = 2;
	y = 1;
	pVtx[12].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[13].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[14].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[15].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 底
	x = 1;
	y = 2;
	pVtx[16].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[17].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[18].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[19].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 左
	x = 0;
	y = 1;
	pVtx[20].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[21].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[22].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[23].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;
}
