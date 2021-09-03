//=============================================================================
//
// 空 [Sky.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Sky.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SKY_NUM_X		(13)						// 横に並べる数
#define SKY_NUM_Y		(3)							// 縦に並べる数

//=============================================================================
// コンストラクタ
//=============================================================================
Sky::Sky()
{
	ResourceManager::Instance()->GetTexture("Sky", &D3DTexture);

	// 位置・回転・スケールの初期設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 30.0f);

	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
Sky::~Sky()
{
	// 頂点バッファの開放
	SAFE_RELEASE(this->D3DVtxBuff);

	// テクスチャの参照を終了
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void Sky::Update()
{

}

//=============================================================================
// 描画
//=============================================================================
void Sky::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	D3DXVECTOR3 oldpos = pos;

	// αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int cntY = 0; cntY < SKY_NUM_Y; cntY++)
	{
		for (int cntX = 0; cntX < SKY_NUM_X; cntX++)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャのセット
			pDevice->SetTexture(0, D3DTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			pos.x += SCREEN_WIDTH;
		}

		pos.x -= SCREEN_WIDTH * SKY_NUM_X;
		pos.y -= SCREEN_HEIGHT;
	}
	pos = oldpos;

	// αテストを無効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点座標の作成
//=============================================================================
HRESULT Sky::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		Vertex3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);

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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}


	return S_OK;
}