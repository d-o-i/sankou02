//=============================================================================
//
// 桁 [Digit.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Digit.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIGIT_SIZE		D3DXVECTOR3(30.0f, 50.0f, 0.0f)
#define DIGIT_SPACE		(40.0f)	// 表示間隔（横）
//#define DIGIT_POS		D3DXVECTOR3(SCREEN_CENTER_X + ((DIGIT_SIZE.x / 2) * DIGIT_MAX), 135.0f, 0.0f)	// 表示場所（1番右、1桁目を基準とする）
#define DIGIT_POS		D3DXVECTOR3((SCREEN_CENTER_X + 120.0f), 135.0f, 0.0f)	// 表示場所（1番右、1桁目を基準とする）

//=============================================================================
// コンストラクタ
//=============================================================================
Digit::Digit(int digit)
{
	ResourceManager::Instance()->GetTexture("Digit", &D3DTexture);
	use = true;
	time = 0;
	pos = DIGIT_POS;
	pos.x -= digit * DIGIT_SPACE;
	// ミリ秒と秒、秒と分の間に':'のスペースがある
	if (digit >= 2)
	{
		pos.x -= 20.0f;
	}
	if (digit >= 4)
	{
		pos.x -= 20.0f;
	}
	size = DIGIT_SIZE;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Digit::~Digit()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void Digit::Update()
{
}

//=============================================================================
// 更新
//=============================================================================
void Digit::Update(DWORD time)
{
	this->time = time;
	SetTexture();
}

//=============================================================================
// 描画
//=============================================================================
void Digit::Draw()
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

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT Digit::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);

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
// テクスチャのセット
//=============================================================================
void Digit::SetTexture()
{
	int x = time % DIGIT_DIVIDE_X;
	int y = time / DIGIT_DIVIDE_X;
	float sizeX = 1.0f / DIGIT_DIVIDE_X;
	float sizeY = 1.0f / DIGIT_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}