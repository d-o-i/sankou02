//=============================================================================
//
// ステージセレクト画面の背景 [StageSelectBG.h]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageSelectBG.h"
#include "ResourceManager.h"
#include "MyLibrary.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGESELECTBG_DIVIDE_X	(1)
#define STAGESELECTBG_DIVIDE_Y	(3)

int StageSelectBG::PatternAnim = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
StageSelectBG::StageSelectBG()
{
	ResourceManager::Instance()->GetTexture("StageSelectBG", &D3DTexture);

	pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	PatternAnim = 0;
	use = true;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
StageSelectBG::~StageSelectBG()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void StageSelectBG::Update()
{
	if (use == true)
	{
		// ステージ切り替え操作
		for (int playerNo = 0; playerNo < GAMEPAD_MAX; playerNo++)
		{
			if (GetKeyboardRepeat(DIK_S) || IsButtonRepeated(playerNo, STICK_DOWN))
			{
				PatternAnim = LoopCountUp(PatternAnim, 0, STAGESELECTBG_DIVIDE_Y - 1);
				break;
			}
			else if (GetKeyboardRepeat(DIK_W) || IsButtonRepeated(playerNo, STICK_UP))
			{
				PatternAnim = LoopCountDown(PatternAnim, 0, STAGESELECTBG_DIVIDE_Y - 1);
				break;
			}
		}

		SetTexture();

		SetVertex();
	}
}

//=============================================================================
// 描画
//=============================================================================
void StageSelectBG::Draw()
{
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
// 頂点情報の作成
//=============================================================================
void StageSelectBG::MakeVertex()
{
	// 頂点座標の設定
	SetVertex();

	// rhwの設定
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
	SetTexture();
}

//=============================================================================
// テクスチャ座標のセット
//=============================================================================
void StageSelectBG::SetTexture()
{
	int x = PatternAnim % STAGESELECTBG_DIVIDE_X;
	int y = PatternAnim / STAGESELECTBG_DIVIDE_X;
	float sizeX = 1.0f / STAGESELECTBG_DIVIDE_X;
	float sizeY = 1.0f / STAGESELECTBG_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標のセット
//=============================================================================
void StageSelectBG::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}