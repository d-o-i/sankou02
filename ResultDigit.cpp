//=============================================================================
//
// リザルト画面での桁 [ResultDigit.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultDigit.h"
#include "ResultRank.h"
#include "Player.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULTDIGIT_POS			D3DXVECTOR3(450.0f, 178.0f, 0.0f)
#define RESULTDIGIT_SIZE		D3DXVECTOR3(40.0f, 85.0f, 0.0f)
#define RESULTDIGIT_INTERVAL	(80.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
ResultDigit::ResultDigit(DWORD _time, int _digit, int _rank) : Digit(_digit)
{
	ResourceManager::Instance()->GetTexture("Digit", &D3DTexture);

	time = _time;
	use = true;
	rank = _rank;
	pos = RESULTDIGIT_POS + D3DXVECTOR3(0.0f, RESULTDIGIT_INTERVAL * rank, 0.0f);
	size = RESULTDIGIT_SIZE;
	pos.x -= _digit * size.x;
	// ミリ秒と秒、秒と分の間に':'のスペースがある
	if (_digit >= 2)
	{
		pos.x -= 20.0f;
	}
	if (_digit >= 4)
	{
		pos.x -= 20.0f;
	}

	MakeVertex();
}


//=============================================================================
// デストラクタ
//=============================================================================
ResultDigit::~ResultDigit()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void ResultDigit::Update()
{
}

//=============================================================================
// 描画
//=============================================================================
void ResultDigit::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// Zテスト
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

	// Z比較あり
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}