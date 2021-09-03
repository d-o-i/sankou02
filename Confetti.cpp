//=============================================================================
//
// 紙吹雪 [Confetti.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Confetti.h"
#include "Camera.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CONFETTI_FILE		("data/TEXTURE/Confetti.png")
#define CONFETTI_WIDTH		(10.0f)		// 大きさ
#define CONFETTI_HEIGHT		(10.0f)		// 大きさ
#define CONFETTI_RANGE_X	(500.0f)	// 落下範囲
#define CONFETTI_RANGE_Z	(500.0f)	// 落下範囲
#define CONFETTI_FALL_SPEED	(1.0f)		// 落下速度
#define CONFETTI_DIVIDE_X	(4)			// テクスチャ分割数
#define CONFETTI_POS_Y		(500.0f)	// 落下する高さ
#define CONFETTI_MOVE_COUNT	(20)		// 動きをつけるタイミング
#define CONFETTI_MOVE_VALUE	(0.3f)		// 前後左右に動くスピードの最大

//*****************************************************************************
// メンバ変数の初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 Confetti::D3DTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
Confetti::Confetti()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (D3DTexture == NULL)
	{
		D3DXCreateTextureFromFile(Device, CONFETTI_FILE, &D3DTexture);
	}

	// 発生座標、色、テクスチャをランダムで指定する
	pos.x = RandomRange(-CONFETTI_RANGE_X, CONFETTI_RANGE_X);
	pos.y = CONFETTI_POS_Y;
	pos.z = RandomRange(-CONFETTI_RANGE_Z, CONFETTI_RANGE_Z);
	int color = rand() % nColorMax;
	col = MyColor[color];
	texNo = rand() % CONFETTI_DIVIDE_X;

	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	move = D3DXVECTOR3(0.0f, CONFETTI_FALL_SPEED, 0.0f);
	width = CONFETTI_WIDTH;
	height = CONFETTI_HEIGHT;
	use = true;
	moveCnt = 0;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Confetti::~Confetti()
{
	// 頂点バッファの開放
	SAFE_RELEASE(this->D3DVtxBuff);
}

//=============================================================================
// 更新
//=============================================================================
void Confetti::Update()
{
	// 使用しているもののみ更新
	if (use)
	{
		moveCnt++;
		// 一定のタイミングで動き方を変える
		if (moveCnt == CONFETTI_MOVE_COUNT)
		{
			moveCnt = 0;
			move.x = RandomRange(-CONFETTI_MOVE_VALUE, CONFETTI_MOVE_VALUE);
			move.z = RandomRange(-CONFETTI_MOVE_VALUE, CONFETTI_MOVE_VALUE);
		}

		pos -= move;

		// 落下仕切ったら消滅
		if (pos.y < 0 - height)
		{
			use = false;
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void Confetti::Draw()
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
		Device->SetTexture(0, Confetti::D3DTexture);

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
HRESULT Confetti::MakeVertex()
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
	pVtx[0].vtx = D3DXVECTOR3(-width / 2, height / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(width / 2, height / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-width / 2, -height / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(width / 2, -height / 2, 0.0f);

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
	int x = texNo % CONFETTI_DIVIDE_X;
	float sizeX = 1.0f / CONFETTI_DIVIDE_X;

	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;

}