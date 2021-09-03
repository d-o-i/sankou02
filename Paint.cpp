//=============================================================================
//
// ペイント [Paint.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Paint.h"
#include "Camera.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

#define DecAlpha (0.1f)

//*****************************************************************************
// メンバの初期化
//*****************************************************************************
float Paint::HalfSize = PAINT_WIDTH / 2;

//=============================================================================
// コンストラクタ
//=============================================================================
Paint::Paint(int Owner, int PaintColor)
{
	ResourceManager::Instance()->GetTexture("Paint", &D3DTexture);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	width = PAINT_WIDTH;
	height = PAINT_HEIGHT;
	time = 0;
	this->Use = false;
	this->InScreen = true;
	this->NodeID = 0;
	this->Owner = Owner;
	this->PaintColor = PaintColor;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Paint::~Paint()
{
	// 頂点バッファの開放
	SAFE_RELEASE(this->D3DVtxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void Paint::Update()
{
	// 使用しているもののみ更新
	if (Use)
	{
		// 表示時間の更新
		if (time > 0)
		{
			time--;
		}
		// 表示時間を超えたら消滅処理
		else
		{
			// 透明度を減衰値に合わせて追加
			col.a -= DecAlpha;

			if (col.a <= 0.0f)
			{
				// 透明になったら使用をやめる
				col.a = 0.0f;
				Use = false;
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// 色の設定
			SetColor();
		}

#if _DEBUG
#endif
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Paint::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

#if 0
	// 減算合成 レンダリングステートの変更→黒っぽくなる（加算合成は白っぽくなる（255に近づけていくと））
	//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zテスト
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#endif

	// Zテスト
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	//Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	//Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (Use)
	{
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

		// 現在のオブジェクトが画面内を確認
		CheckInScreen(WorldMtx);

		// 頂点バッファをデバイスのデータストリームにバインド
		Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		Device->SetTexture(0, Paint::D3DTexture);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	// αテストを無効に
	//Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z比較あり
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

#if 0
	// 通常ブレンド レンダリングステートをもとに戻す（戻さないと減算合成のままになる）
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT Paint::MakeVertex()
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
	pVtx[0].vtx = D3DXVECTOR3(-PAINT_WIDTH / 2, PAINT_HEIGHT / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(PAINT_WIDTH / 2, PAINT_HEIGHT / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-PAINT_WIDTH / 2, -PAINT_HEIGHT / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(PAINT_WIDTH / 2, -PAINT_HEIGHT / 2, 0.0f);

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
	int x = this->PaintColor % PAINT_DIVIDE_X;
	float sizeX = 1.0f / PAINT_DIVIDE_X;

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
void Paint::SetColor()
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
// ワールド座標からスクリーン座標に変換する
//=============================================================================
void Paint::CheckInScreen(D3DXMATRIX WorldMatrix)
{
	ScreenPos = WorldToScreenPos(WorldMatrix);

	if (ScreenPos.x < 0 ||
		ScreenPos.x > SCREEN_WIDTH ||
		ScreenPos.y < 0 ||
		ScreenPos.y > SCREEN_HEIGHT)
	{
		this->InScreen = false;
	}
	else
	{
		this->InScreen = true;
	}
}
