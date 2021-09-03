//=============================================================================
//
// 3Dビルボードエフェクト処理 [Effect3D.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Effect3D.h"
#include "MyLibrary.h"
#include "Camera.h"
#include "ResourceManager.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// エフェクトデータ構造体
struct EffectData3D
{
	const char *texture;	// テクスチャファイル
	D3DXVECTOR3 size;		// サイズ
	int count;				// 更新フレーム
	Int2D pattern;			// テクスチャ分割数(x,y)
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static EffectData3D EffectData3DWk[EffectMax] =
{
	{ "data/EFFECT/anmef000.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 7, Int2D(5, 1) },
{ "data/EFFECT/anmef001.png", D3DXVECTOR3(300.0f, 300.0f, 0.0f), 7, Int2D(1, 5) },
{ "data/EFFECT/anmef002.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 7, Int2D(2, 2) },
{ "data/EFFECT/explo000.png", D3DXVECTOR3(300.0f, 300.0f, 0.0f), 3, Int2D(5, 3) },
{ "data/EFFECT/ief001.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 10, Int2D(5, 2) },
{ "data/EFFECT/ief000.png", D3DXVECTOR3(70.0f, 70.0f, 0.0f), 4, Int2D(3, 1) },
{ "data/EFFECT/Charge.png", D3DXVECTOR3(75.0f, 75.0f, 0.0f), 10 ,Int2D(2, 7) },
};

//=============================================================================
// コンストラクタ(無限ループさせる場合はINFINITY_LOOPを渡す)
//=============================================================================
Effect3D::Effect3D(int Effect3DNum, D3DXVECTOR3 _pos, int _LoopNum)
{
	D3DVtxBuff = NULL;

	TexNo = Effect3DNum;

	switch (TexNo)
	{
	case HitEffect3D:
		ResourceManager::Instance()->GetTexture("HitEffect", &D3DTexture);
		break;
	case DeadEffect3D:
		ResourceManager::Instance()->GetTexture("DeadEffect", &D3DTexture);
		break;
	case RunEffect3D:
		ResourceManager::Instance()->GetTexture("RunEffect", &D3DTexture);
		break;
	case ExplosionEffect3D:
		ResourceManager::Instance()->GetTexture("ExplosionEffect", &D3DTexture);
		break;
	case ItemEffect3D:
		ResourceManager::Instance()->GetTexture("ItemEffect", &D3DTexture);
		break;
	case Item1Effect3D:
		ResourceManager::Instance()->GetTexture("Item1Effect", &D3DTexture);
		break;
	case ChargeEffect3D:
		ResourceManager::Instance()->GetTexture("ChargeEffect", &D3DTexture);
		break;
	default:
		break;
	}

	use = true;
	CountAnim = 0;
	PatternAnim = 0;
	size = EffectData3DWk[TexNo].size;
	pos = _pos;
	xPattern = EffectData3DWk[TexNo].pattern.x;
	yPattern = EffectData3DWk[TexNo].pattern.y;
	TexAnimNum = xPattern * yPattern;
	AnimationCnt = EffectData3DWk[TexNo].count;
	loopnum = _LoopNum;
	loopcnt = 0;

	// 頂点情報の作成
	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
Effect3D::~Effect3D()
{
	if (D3DVtxBuff != NULL)
	{
		D3DVtxBuff->Release();
		D3DVtxBuff = NULL;
	}

	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void Effect3D::Update()
{
	if (use == true)
	{
		// アニメーション
		CountAnim++;

		if ((CountAnim % AnimationCnt) == 0)
		{
			// パターンの切り替え
			PatternAnim = LoopCountUp(PatternAnim, 0, TexAnimNum);

			// ループ処理
			Loop();

			//テクスチャ座標をセット
			SetTexture();

		}

	}
}

//=============================================================================
// 描画
//=============================================================================
void Effect3D::Draw()
{
	if (use)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// Zテストを有効に
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

		// αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// Z比較を無効に
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	}
}

//=============================================================================
// エフェクトのループ処理
//=============================================================================
void Effect3D::Loop()
{

	// 無限ループのとき以外
	if (loopnum != INFINITY_LOOP)
	{
		if (PatternAnim == 0)
		{
			loopcnt++;

			// 指定のループ回数に達したらエフェクトを終了
			if (loopcnt == loopnum)
			{
				use = false;
				loopcnt = 0;
			}
		}
	}

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT Effect3D::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		FVF_VERTEX_3D,														// 使用する頂点フォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,														// 頂点バッファインターフェースへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		Vertex3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-size.x / 2.0f, size.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(size.x / 2.0f, size.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-size.x / 2.0f, -size.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(size.x / 2.0f, -size.y / 2.0f, 0.0f);

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
		int x = PatternAnim % xPattern;
		int y = PatternAnim / xPattern;
		float sizeX = 1.0f / xPattern;
		float sizeY = 1.0f / yPattern;

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void Effect3D::SetTexture()
{
	{//頂点バッファの中身を埋める
		Vertex3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		int x = PatternAnim % xPattern;
		int y = PatternAnim / xPattern;
		float sizeX = 1.0f / xPattern;
		float sizeY = 1.0f / yPattern;

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}

}
