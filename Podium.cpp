//=============================================================================
//
// 表彰台[Podium.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Podium.h"

#define PODIUM_MODEL	("data/MODEL/Podium/Podium.x")
#define PODIUM_ROT		D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
Podium::Podium()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this->Load_xFile(PODIUM_MODEL, "Podium");

	// アニメーションセットの初期化
	this->CreateAnimSet();

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = PODIUM_ROT;
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

//=============================================================================
// デストラクタ
//=============================================================================
Podium::~Podium()
{
}

//=============================================================================
// 更新
//=============================================================================
void Podium::Update()
{

}

//=============================================================================
// 描画
//=============================================================================
void Podium::Draw()
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
void Podium::CreateAnimSet()
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
HRESULT CALLBACK Podium::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}
