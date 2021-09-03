//=============================================================================
//
// トロフィー[Trophy.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Trophy.h"

#define TROPHY_MODEL	("data/MODEL/Trophy.x")
#define TROPHY_POS		D3DXVECTOR3(0.0f, 150.0f, 0.0f)
#define TROPHY_ROT		D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(30))
#define TROPHY_MOVE		D3DXVECTOR3(0.0f, D3DXToRadian(1), 0.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
Trophy::Trophy()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this->Load_xFile(TROPHY_MODEL, "Podium");

	// アニメーションセットの初期化
	this->CreateAnimSet();

	pos = TROPHY_POS;
	rot = D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(30));
	scl = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
}

//=============================================================================
// デストラクタ
//=============================================================================
Trophy::~Trophy()
{
}

//=============================================================================
// 更新
//=============================================================================
void Trophy::Update()
{
	// 回転させる
	rot += TROPHY_MOVE;
	if (rot.y >= D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}
}

//=============================================================================
// 描画
//=============================================================================
void Trophy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	D3DXMATRIX WorldMtx, SclMtx, RotMtx, TransMtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// スケールを反映
	D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&SclMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// 移動を反映
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// レンダリング
	this->DrawAnim(&WorldMtx);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// アニメーションセットの作成
//=============================================================================
void Trophy::CreateAnimSet()
{
	ANIMATIONSET *AnimationSet = new ANIMATIONSET();
	vector<KEYDATA>Keydata;
	Keydata.reserve(Keydata_Max);
	AnimationSet->SetData("Idle", NULL, 1.0f, 0.1f, 0.0f);
	this->SetupCallbackKeys(&Keydata, AnimationSet->GetSetName());
	AnimationSet->SetAnimSetPtr(this->AnimController);
	this->AnimSet.push_back(*AnimationSet);
	Keydata.clear();
	SAFE_DELETE(AnimationSet);
	ReleaseVector(Keydata);

}

//=====================================================================================================
// アニメーションCallbackKeyの処理イベント
//=====================================================================================================
HRESULT CALLBACK Trophy::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}
