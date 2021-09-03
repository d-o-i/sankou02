//=============================================================================
//
// パーティクル [Particle.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Particle.h"
#include "Camera.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_FILE	("data/TEXTURE/Effect.jpg")

//*****************************************************************************
// メンバ変数の初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 Particle::D3DTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
Particle::Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scl, int time)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (D3DTexture == NULL)
	{
		D3DXCreateTextureFromFile(Device, EFFECT_FILE, &D3DTexture);
	}

	// 発生座標、色、テクスチャをランダムで指定する
	this->pos = pos;
	this->scl = scl;
	this->move = move;
	this->col = col;
	this->time = time;
	this->decAlpha = (float)this->col.a / (float)this->time;
	this->use = true;
	this->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Particle::~Particle()
{
	// 頂点バッファの開放
	SAFE_RELEASE(this->D3DVtxBuff);
}

//=============================================================================
// 更新
//=============================================================================
void Particle::Update()
{
	if (!use)
		return;

	this->pos += this->move;

	// 透明度の増加
	this->col.a -= this->decAlpha;
	if (this->col.a <= 0)
	{
		this->use = false;
	}

	SetTexture();
	SetVertex();
	SetColor();
}

//=============================================================================
// 描画
//=============================================================================
void Particle::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

	// ライティングを無効にする
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

#if 0
	// 減算合成 レンダリングステートの変更→黒っぽくなる（加算合成は白っぽくなる（255に近づけていくと））
	//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zテスト
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
#endif
	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (use)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&WorldMtx);

		// ビューマトリックスを取得
		ViewMtx = cameraWk->mtxView;

		// ポリゴンを正面に向ける
		WorldMtx._11 = ViewMtx._11;
		WorldMtx._12 = ViewMtx._21;
		WorldMtx._13 = ViewMtx._31;
		WorldMtx._21 = ViewMtx._12;
		WorldMtx._22 = ViewMtx._22;
		WorldMtx._23 = ViewMtx._32;
		WorldMtx._31 = ViewMtx._13;
		WorldMtx._32 = ViewMtx._23;
		WorldMtx._33 = ViewMtx._33;

		// 逆行列をもとめる
		D3DXMatrixInverse(&WorldMtx, NULL, &ViewMtx);
		WorldMtx._41 = 0.0f;
		WorldMtx._42 = 0.0f;
		WorldMtx._43 = 0.0f;

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
		Device->SetTexture(0, Particle::D3DTexture);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	// αテストを無効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z比較あり
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

#if 1
	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif

	// ラインティングを有効にする
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT Particle::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-scl.x / 2, scl.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(scl.x / 2, scl.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-scl.x / 2, -scl.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(scl.x / 2, -scl.y / 2, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;

}

//=============================================================================
// 頂点座標のセット
//=============================================================================
void Particle::SetVertex()
{
	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-scl.x / 2, scl.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(scl.x / 2, scl.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-scl.x / 2, -scl.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(scl.x / 2, -scl.y / 2, 0.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

}

//=============================================================================
// 反射光のセット
//=============================================================================
void Particle::SetColor()
{
	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

}

//=============================================================================
// テクスチャ座標のセット
//=============================================================================
void Particle::SetTexture()
{
	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

}