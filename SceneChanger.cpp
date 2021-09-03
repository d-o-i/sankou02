//=============================================================================
//
// シーン切り替え [SceneChanger.h]
// Author : 
// 
//=============================================================================
#include "Main.h"
#include "SceneChanger.h"
#include "ResourceManager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
SceneChanger::SceneChanger()
{
	pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	radius = D3DXVec2Length(&D3DXVECTOR2((float)SCREEN_WIDTH / 2, (float)SCREEN_WIDTH / 2));
	angle = atan2f((float)SCREEN_WIDTH, (float)SCREEN_WIDTH);

	callback = NULL;

	useMask = false;
	useChanger = false;

	MakeVertexMask();
	MakeVertexChanger();
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneChanger::~SceneChanger()
{
	maskTexture = NULL;
	changeTexture = NULL;
}

//=============================================================================
// マスク開始
//=============================================================================
void SceneChanger::BeginMask()
{
	// すでに開始している場合リターン
	if (!useMask)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 0.0f, 0);
}

//=============================================================================
// マスク描画
//=============================================================================
void SceneChanger::DrawMask()
{
	if (!useMask)
		return;

	// 最小になったら終了
	if (scl == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		false;

	BeginMask();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, maskTexture);

	pDevice->SetFVF(FVF_VERTEX_2D);

	SetVertexMask();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkMask, sizeof(Vertex2D));

	EndMask();

}

//=============================================================================
// マスク終了
//=============================================================================
void SceneChanger::EndMask()
{
	if (!useMask)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED |
		D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE |
		D3DCOLORWRITEENABLE_ALPHA);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

}

//=============================================================================
// チェンジャーの描画
//=============================================================================
void SceneChanger::DrawChanger()
{
	if (!useChanger)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x00);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetTexture(0, changeTexture);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkChange, sizeof(Vertex2D));
	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);

}

//=============================================================================
// マスク用テクスチャ読み込み
//=============================================================================
void SceneChanger::LoadMaskTexture()
{
	ResourceManager::Instance()->GetTexture("Mask", &maskTexture);
}

//=============================================================================
// チェンジャー用テクスチャ読み込み
//=============================================================================
void SceneChanger::LoadChangeTexture()
{
	ResourceManager::Instance()->GetTexture("Changer", &changeTexture);
}

//=============================================================================
// 頂点情報のセット
//=============================================================================
void SceneChanger::SetVertexMask()
{
	vertexWkMask[0].vtx.x = pos.x - cosf(angle + rot.z) * radius * scl.x;
	vertexWkMask[0].vtx.y = pos.y - sinf(angle + rot.z) * radius * scl.y;
	vertexWkMask[1].vtx.x = pos.x + cosf(angle - rot.z) * radius * scl.x;
	vertexWkMask[1].vtx.y = pos.y - sinf(angle - rot.z) * radius * scl.y;
	vertexWkMask[2].vtx.x = pos.x - cosf(angle - rot.z) * radius * scl.x;
	vertexWkMask[2].vtx.y = pos.y + sinf(angle - rot.z) * radius * scl.y;
	vertexWkMask[3].vtx.x = pos.x + cosf(angle + rot.z) * radius * scl.x;
	vertexWkMask[3].vtx.y = pos.y + sinf(angle + rot.z) * radius * scl.y;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SceneChanger::MakeVertexMask()
{
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWkMask[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWkMask[i].rhw = 1.0f;
	}

	vertexWkMask[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkMask[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkMask[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkMask[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	SetVertexMask();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SceneChanger::MakeVertexChanger()
{
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWkChange[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWkChange[i].rhw = 1.0f;
	}

	vertexWkChange[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWkChange[1].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWkChange[2].vtx = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 0.0f);
	vertexWkChange[3].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	vertexWkChange[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkChange[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkChange[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkChange[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}