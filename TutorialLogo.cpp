//=============================================================================
//
// チュートリアル画面のロゴ [TitleLogo.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "TutorialLogo.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIALLOGO_POS	D3DXVECTOR3((float)SCREEN_CENTER_X, 100.0f, 0.0f)
#define TUTORIALLOGO_SIZE	D3DXVECTOR3(512.0f, 128.0f, 0.0f)
#define FLASH_TIME			(45)
#define DELETE_TIME			(30)

//=============================================================================
// コンストラクタ
//=============================================================================
TutorialLogo::TutorialLogo()
{
	ResourceManager::Instance()->GetTexture("TutorialLogo", &D3DTexture);

	pos = TUTORIALLOGO_POS;
	size = TUTORIALLOGO_SIZE;
	use = true;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
TutorialLogo::~TutorialLogo()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void TutorialLogo::Update()
{
	if (!use)
		return;

	// 点滅処理
	cntFlash++;

	// 表示中に一定フレーム経過で消滅
	if (flash && cntFlash == FLASH_TIME)
	{
		flash = false;
		cntFlash = 0;
	}
	// 消滅中に一定フレーム経過で表示
	else if (!flash && cntFlash == DELETE_TIME)
	{
		flash = true;
		cntFlash = 0;
	}

}

//=============================================================================
// 描画
//=============================================================================
void TutorialLogo::Draw()
{
	// 使用していないor点滅中は表示しない
	if (!use || !flash)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, D3DTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void TutorialLogo::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		// rhwの設定
		vertexWk[i].rhw = 1.0f;
		// 反射光の設定
		vertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}