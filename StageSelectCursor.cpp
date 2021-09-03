//=============================================================================
//
// ステージセレクト用カーソル [StageSelectCursor.h]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageSelectCursor.h"
#include "Input.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGESELECTCURSOR_POS	D3DXVECTOR3(60.0f, 447.0f, 0.0f)
#define CURSOR_MOVE_VALUE		(93.0f)
#define CURSOR_SIZE				D3DXVECTOR3(150.0f, 150.0f, 0.0f)
#define MAXPOS_Y				(STAGESELECTCURSOR_POS.y + CURSOR_MOVE_VALUE * 2)

//=============================================================================
// コンストラクタ
//=============================================================================
StageSelectCursor::StageSelectCursor()
{
	ResourceManager::Instance()->GetTexture("TitleCursor", &D3DTexture);

	pos = STAGESELECTCURSOR_POS;
	size = CURSOR_SIZE;
	use = true;

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
StageSelectCursor::~StageSelectCursor()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void StageSelectCursor::Update()
{
	if (use)
	{
		for (int ctrNo = 0; ctrNo < GAMEPAD_MAX; ctrNo++)
		{
			if (GetKeyboardRepeat(DIK_S) || IsButtonRepeated(ctrNo, STICK_DOWN))
			{
				pos.y == MAXPOS_Y ? pos.y = STAGESELECTCURSOR_POS.y : pos.y += CURSOR_MOVE_VALUE;
				break;
			}
			else if (GetKeyboardRepeat(DIK_W) || IsButtonRepeated(ctrNo, STICK_UP))
			{
				pos.y == STAGESELECTCURSOR_POS.y ? pos.y = MAXPOS_Y : pos.y -= CURSOR_MOVE_VALUE;
				break;
			}
		}

		SetVertex();
	}
}

//=============================================================================
// 描画
//=============================================================================
void StageSelectCursor::Draw()
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
void StageSelectCursor::MakeVertex()
{
	// 頂点座標の設定
	SetVertex();

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].rhw = 1.0f;
		vertexWk[i].diffuse = D3DXCOLOR(1.00f, 1.00f, 1.00f, 1.00f);
	}

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標のセット
//=============================================================================
void StageSelectCursor::SetVertex()
{
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}