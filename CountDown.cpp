//=============================================================================
//
// カウントダウン処理 [CountDown.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "CountDown.h"
#include "Sound.h"
#include "ResourceManager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTDOWN_SIZE		D3DXVECTOR2(360.0f, 360.0f)
#define COUNTDOWN_DIVIDE_Y	(4)
#define COUNTDOWN_POS		D3DXVECTOR3(SCREEN_WIDTH / 2 - COUNTDOWN_SIZE.x / 2, SCREEN_HEIGHT / 2 - COUNTDOWN_SIZE.y / 2, 0.0f)
#define START_TIMER			(240)
#define SECOND_PER_FRAME	(60)

//=============================================================================
// コンストラクタ
//=============================================================================
CountDown::CountDown()
{
	ResourceManager::Instance()->GetTexture("CountDown", &D3DTexture);

	use = true;
	pos = COUNTDOWN_POS;
	starttimer = 0;
	startsecond = 0;

	// 頂点情報の作成
	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
CountDown::~CountDown()
{
	// テクスチャの開放
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void CountDown::Update()
{
	if (use)
	{
		if (starttimer == 0)
		{
			PlaySound(SE_COUNT);
		}
		if (starttimer < START_TIMER)
		{
			starttimer++;
			startsecond = starttimer / SECOND_PER_FRAME;

			SetTexture();

		}
		// カウントダウン終了
		else
		{
			use = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CountDown::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// Zテストを有効に
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		Device->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

	// αテストを無効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z比較を無効に
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT CountDown::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 1.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + COUNTDOWN_SIZE.x, pos.y, 1.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + COUNTDOWN_SIZE.y, 1.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + COUNTDOWN_SIZE.x, pos.y + COUNTDOWN_SIZE.y, 1.0f);

	// rhwの設定
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標のセット
//=============================================================================
void CountDown::SetTexture()
{
	float sizeY = 1.0f / COUNTDOWN_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, (float)(startsecond)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, (float)(startsecond)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, (float)(startsecond)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, (float)(startsecond)* sizeY + sizeY);
}