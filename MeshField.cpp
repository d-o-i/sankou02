//=============================================================================
//
// メッシュ地面の処理 [Meshfield.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MeshField.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)	// 表示位置
#define MESHFIELD_ROT			D3DXVECTOR3(0.0f, 0.0f, 0.0f)	// 回転
#define MESHFIELD_NUM_BLOCK_X	(10)							// ブロック数
#define MESHFIELD_NUM_BLOCK_Z	(10)
#define MESHFIELD_SIZE			D3DXVECTOR3(80.0f, 0.0f, 80.0f)	// サイズ

//=============================================================================
// コンストラクタ
//=============================================================================
MeshField::MeshField()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ResourceManager::Instance()->GetTexture("MeshField", &D3DTexture);

	// ポリゴン表示位置の中心座標を設定
	pos = MESHFIELD_POS;
	rot = MESHFIELD_ROT;

	// ブロック数の設定
	NumBlockX = MESHFIELD_NUM_BLOCK_X;
	NumBlockZ = MESHFIELD_NUM_BLOCK_Z;

	// 頂点数の設定
	NumVertex = (MESHFIELD_NUM_BLOCK_X + 1) * (MESHFIELD_NUM_BLOCK_Z + 1);

	// インデックス数の設定
	NumVertexIndex = (MESHFIELD_NUM_BLOCK_X + 1) * 2 * MESHFIELD_NUM_BLOCK_Z + (MESHFIELD_NUM_BLOCK_Z - 1) * 2;

	// ポリゴン数の設定
	NumPolygon = MESHFIELD_NUM_BLOCK_X * MESHFIELD_NUM_BLOCK_Z * 2 + (MESHFIELD_NUM_BLOCK_Z - 1) * 4;

	// ブロックサイズの設定
	BlockSizeX = MESHFIELD_SIZE.x;
	BlockSizeZ = MESHFIELD_SIZE.z;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
		NULL);

	// オブジェクトのインデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * NumVertexIndex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&D3DIdxBuff,						// インデックスバッファインターフェースへのポインタ
		NULL);

	{//頂点バッファの中身を埋める
		Vertex3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (NumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (NumBlockX + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.x = -(NumBlockX / 2.0f) * BlockSizeX + nCntVtxX * BlockSizeX;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.z = (NumBlockZ / 2.0f) * BlockSizeZ - nCntVtxZ * BlockSizeZ;

				// 法線の設定
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// 反射光の設定
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		D3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < NumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (NumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (NumBlockZ - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockX + 1) + NumBlockX;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		D3DIdxBuff->Unlock();
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
MeshField::~MeshField()
{

	SAFE_RELEASE(D3DVtxBuff);
	SAFE_RELEASE(D3DIdxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void MeshField::Update()
{

}

//=============================================================================
// 描画
//=============================================================================
void MeshField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX WorldMtx, RotMtx, TransMtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&RotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &RotMtx);

	// 移動を反映
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(D3DIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, D3DTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertex, 0, NumPolygon);
}