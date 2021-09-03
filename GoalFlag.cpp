//=============================================================================
//
// ゴール地点表示 [GoalFlag.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "GoalFlag.h"
#include "Map.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "StageSelectBG.h"
#include "DebugWindow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GOAL_SIZE		D3DXVECTOR3(12.8f, 12.8f, 0.0f)
#define FLAG_POS		D3DXVECTOR3(GOAL_POS.x, -88.0f, -10.0f)

D3DXVECTOR3 GoalFlagPos[] = {
	D3DXVECTOR3(GOAL_POS.x, -530.0f, 0.0f),
	D3DXVECTOR3(GOAL_POS.x, -270.0f, 0.0f),
	D3DXVECTOR3(GOAL_POS.x, -470.0f, 0.0f),
};

//=============================================================================
// コンストラクタ
//=============================================================================
GoalFlag::GoalFlag()
{
	ResourceManager::Instance()->GetTexture("GoalFlag", &D3DTexture);

	pos = GoalFlagPos[StageSelectBG::GetStageSelect()];
	scl = GOAL_SIZE;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
GoalFlag::~GoalFlag()
{
	D3DTexture = NULL;
	SAFE_RELEASE(D3DVtxBuff);
}

//=============================================================================
// 更新
//=============================================================================
void GoalFlag::Update()
{

}

//=============================================================================
// 描画
//=============================================================================
void GoalFlag::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

	// Zテスト
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// スケールを反映
	D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// 移動を反映
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &WorldMtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, D3DTexture);

	// ポリゴンの描画
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// αテストを無効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z比較あり
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT GoalFlag::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-GOAL_SIZE.x / 2, GOAL_SIZE.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(GOAL_SIZE.x / 2, GOAL_SIZE.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-GOAL_SIZE.x / 2, -GOAL_SIZE.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(GOAL_SIZE.x / 2, -GOAL_SIZE.y / 2, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuff->Unlock();

	return S_OK;
}