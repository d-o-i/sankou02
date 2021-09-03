//=============================================================================
//
// エフェクト処理 [Effect.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Effect.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// エフェクトデータ構造体
struct EffectData
{
	const char *texture;	// テクスチャファイル
	D3DXVECTOR3 size;		// サイズ
	int count;				// 更新フレーム
	Int2D pattern;			// テクスチャ分割数(x,y)
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static EffectData EffectDataWk[EffectMax] =
{
	{ "data/EFFECT/Hit.png", D3DXVECTOR3(250.0f, 250.0f, 0.0f), 4, Int2D(2, 5) },
{ "data/EFFECT/anmef001.png", D3DXVECTOR3(500.0f, 100.0f, 0.0f), 4, Int2D(1, 5) },
{ "data/EFFECT/anmef002.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 4, Int2D(2, 2) },
{ "data/EFFECT/explo000.png", D3DXVECTOR3(75.0f, 75.0f, 0.0f), 4, Int2D(5, 3) },
{ "data/EFFECT/ief001.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 10, Int2D(5, 2) },
{ "data/EFFECT/ief000.png", D3DXVECTOR3(70.0f, 70.0f, 0.0f), 4, Int2D(3, 1) },
{ "data/EFFECT/Charge.png", D3DXVECTOR3(75.0f, 75.0f, 0.0f), 10 ,Int2D(2, 7) },
};

//=============================================================================
// コンストラクタ(無限ループさせる場合はINFINITY_LOOPを渡す)
//=============================================================================
Effect::Effect(EffectNum num, D3DXVECTOR3 _pos, int _LoopNum)
{
	
	D3DVtxBuff = NULL;

	TexNo = num;

	switch (TexNo)
	{
	case HitEffect:
		ResourceManager::Instance()->GetTexture("HitEffect", &D3DTexture);
		break;
	case DeadEffect:
		ResourceManager::Instance()->GetTexture("DeadEffect", &D3DTexture);
		break;
	case RunEffect:
		ResourceManager::Instance()->GetTexture("RunEffect", &D3DTexture);
		break;
	case ExplosionEffect:
		ResourceManager::Instance()->GetTexture("ExplosionEffect", &D3DTexture);
		break;
	case ItemEffect:
		ResourceManager::Instance()->GetTexture("ItemEffect", &D3DTexture);
		break;
	case Item1Effect:
		ResourceManager::Instance()->GetTexture("Item1Effect", &D3DTexture);
		break;
	case ChargeEffect:
		ResourceManager::Instance()->GetTexture("ChargeEffect", &D3DTexture);
		break;
	default:
		break;
	}

	// フレームの初期化
	use = true;
	CountAnim = 0;
	PatternAnim = 0;
	size = EffectDataWk[TexNo].size;
	pos = _pos;
	xPattern = EffectDataWk[TexNo].pattern.x;
	yPattern = EffectDataWk[TexNo].pattern.y;
	TexAnimNum = xPattern * yPattern;
	AnimationCnt = EffectDataWk[TexNo].count;
	loopnum = _LoopNum;
	loopcnt = 0;

	// 頂点情報の作成
	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
Effect::~Effect()
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
void Effect::Update()
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
			SetTexture(PatternAnim);

		}

	}

	// 頂点座標の設定
	SetVertex();

}

//=============================================================================
// 描画
//=============================================================================
void Effect::Draw() {

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// エフェクトのループ処理
//=============================================================================
void Effect::Loop()
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
HRESULT Effect::MakeVertex(void) {

	// 頂点座標の設定
	SetVertex();

	// テクスチャのパースペクティブコレクト用
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
void Effect::SetTexture(int cntPattern) {

	int x = cntPattern % xPattern;
	int y = cntPattern / xPattern;
	float sizeX = 1.0f / xPattern;
	float sizeY = 1.0f / yPattern;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void Effect::SetVertex(void) {

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);

}