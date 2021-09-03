//=============================================================================
//
// フィールド上に生成したアイテム [FieldItem.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "FieldItem.h"
#include "Item.h"
#include "Camera.h"
#include "ResourceManager.h"

//*****************************************************************************
// メンバの初期化
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9	FieldItem::D3DVtxBuff = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
FieldItem::FieldItem(int _texno, D3DXVECTOR3 _pos, D3DXVECTOR3 _move)
{
	ResourceManager::Instance()->GetTexture("FieldItem", &D3DTexture);

	pos = _pos;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = _move;
	width = FIELDITEM_SIZE.x;
	height = FIELDITEM_SIZE.y;
	texno = _texno;
	use = false;

	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
FieldItem::~FieldItem()
{
	// 頂点バッファの開放
	SAFE_RELEASE(D3DVtxBuff);
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void FieldItem::Update()
{
	if (use)
	{
		pos += move;

		OnCamera();
	}
}

//=============================================================================
// 描画
//=============================================================================
void FieldItem::Draw()
{
	if (use)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		CAMERA *cameraWk = GetCamera();
		D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

		// ラインティングを無効にする
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 減算合成 レンダリングステートの変更→黒っぽくなる（加算合成は白っぽくなる（255に近づけていくと））
		//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
		//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		// Zテスト
		//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

#if 0
	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
		Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#endif
		// Zテスト
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		// αテストを有効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&WorldMtx);

		// スケールを反映
		D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
		D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

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
		Device->SetTexture(0, FieldItem::D3DTexture);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ラインティングを有効にする
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		// αテストを無効に
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
#if 0
		// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
		Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif
		// Z比較あり
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT FieldItem::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
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
	pVtx[0].vtx = D3DXVECTOR3(-FIELDITEM_SIZE.x / 2, FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(FIELDITEM_SIZE.x / 2, FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-FIELDITEM_SIZE.x / 2, -FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(FIELDITEM_SIZE.x / 2, -FIELDITEM_SIZE.y / 2, 0.0f);

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
	int x = texno % DIVIDE_ITEM_X;
	float sizeX = 1.0f / DIVIDE_ITEM_X;

	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void FieldItem::SetColor()
{
	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();
}

//=============================================================================
// 画面内判定
//=============================================================================
void FieldItem::OnCamera()
{
	CAMERA *camera = GetCamera();

	// 画面内判定
	// 縦
	if ((pos.x > camera->at.x - DRAW_RANGE.x) && (pos.x < camera->at.x + DRAW_RANGE.x))
	{
		// 横
		if ((pos.y > camera->at.y - DRAW_RANGE.y) && (pos.y < camera->at.y + DRAW_RANGE.y))
		{
			use = true;
		}
		else
		{
			use = false;
		}
	}
	else
	{
		use = false;
	}
}
