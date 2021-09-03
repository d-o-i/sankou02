//=============================================================================
//
// チュートリアル画面 [Tutorial.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Tutorial.h"
#include "ResourceManager.h"
#include "Input.h"
#include "DebugWindow.h"

#define TUTORIAL_POS	D3DXVECTOR3((float)SCREEN_CENTER_X * 3, (float)SCREEN_CENTER_Y, 0.0f)
#define TUTORIAL_SIZE	D3DXVECTOR3((float)SCREEN_WIDTH * 3, (float)SCREEN_HEIGHT, 0.0f)
#define TUTORIAL_DIVIDE_X	(1)
#define TUTORIAL_DIVIDE_Y	(1)
#define TUTORIAL_DIVIDE		(3)
#define MOVE_FRAME			(20)

int Tutorial::slideNo = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
Tutorial::Tutorial()
{
	ResourceManager::Instance()->GetTexture("Tutorial", &D3DTexture);

	pos = TUTORIAL_POS;
	newPos = pos;
	size = TUTORIAL_SIZE;
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	use = true;
	PatternAnim = 0;
	slideNo = 0;
	moveFrame = 0;

	MakeVertex();

}

//=============================================================================
// デストラクタ
//=============================================================================
Tutorial::~Tutorial()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新
//=============================================================================
void Tutorial::Update()
{
	if (!use)
		return;

	if (pos == newPos)
	{
		if (GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, STICK_RIGHT))
		{
			slideNo = min(slideNo++, TUTORIAL_DIVIDE - 1);
			newPos.x = TUTORIAL_POS.x - slideNo * (float)SCREEN_WIDTH;
			move.x = (newPos.x - pos.x) / (float)MOVE_FRAME;
			moveFrame = 0;
		}
		else if (GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, STICK_LEFT))
		{
			slideNo = max(slideNo--, 0);
			newPos.x = TUTORIAL_POS.x - slideNo * (float)SCREEN_WIDTH;
			move.x = (newPos.x - pos.x) / (float)MOVE_FRAME;
			moveFrame = 0;
		}
	}

	// 移動判定
	moveFrame++;
	if (moveFrame < MOVE_FRAME)
	{
		pos += move;
	}
	else
	{
		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		moveFrame = 0;
		pos.x = TUTORIAL_POS.x - slideNo * (float)SCREEN_WIDTH;
	}

	SetTexture();
	SetVertex();

#if _DEBUG
	Debug();
#endif
}

//=============================================================================
// 描画
//=============================================================================
void Tutorial::Draw()
{
	// 使用していないor点滅中は表示しない
	if (!use)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, D3DTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void Tutorial::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertex();

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		// rhwの設定
		vertexWk[i].rhw = 1.0f;
		// 反射光の設定
		vertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャ座標の設定
	SetTexture();
}


//=============================================================================
// 頂点座標のセット
//=============================================================================
void Tutorial::SetVertex()
{
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// テクスチャ座標のセット
//=============================================================================
void Tutorial::SetTexture()
{
	int x = PatternAnim % TUTORIAL_DIVIDE_X;
	int y = PatternAnim / TUTORIAL_DIVIDE_X;
	float sizeX = 1.0f / TUTORIAL_DIVIDE_X;
	float sizeY = 1.0f / TUTORIAL_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

void Tutorial::Debug()
{
	BeginDebugWindow("Tutorial");

	DebugText("SlideNo %d", slideNo);
	DebugText("MoveFrame %d", moveFrame);
	DebugText("move %f %f %f", move.x, move.y, move.z);
	DebugText("pos %f %f %f", pos.x, pos.y, pos.z);
	DebugText("newpos %f %f %f", newPos.x, newPos.y, newPos.z);

	EndDebugWindow("Tutorial");
}