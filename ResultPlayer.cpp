//=============================================================================
//
// リザルト画面でのプレイヤー [ResultPlayer.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultPlayer.h"
#include "Player.h"
#include "ResourceManager.h"
#include "SceneCharacterSelect.h"

//*****************************************************************************
// データ定義
//*****************************************************************************
D3DXVECTOR3 ResultPos[PLAYER_MAX] = {
	D3DXVECTOR3(0.0f, 60.0f, 0.0f),
	D3DXVECTOR3(-100.0f, 40.0f, 0.0f),
	D3DXVECTOR3(80.0f, 30.0f, 0.0f),
	D3DXVECTOR3(160.0f, 0.0f, 0.0f)
};

//=============================================================================
// コンストラクタ
//=============================================================================
ResultPlayer::ResultPlayer(int rank, int owner)
{
	// xFileを読み込む
	this->Load_xFile(CharaModel[SceneCharacterSelect::GetSelectCharacter(owner)], "Player");

	// アニメーションセットを設置する
	this->CreateAnimSet();

	// 現在のアニメーションを順位によって決定する
	switch (rank)
	{
	case 0:
		this->ChangeAnim(Victory);
		break;
	case 1:
	case 2:
		this->ChangeAnim(Clapping);
		break;
	case 3:
		this->ChangeAnim(Lose);
		break;
	default:
		break;
	}

	pos = ResultPos[rank];
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
ResultPlayer::~ResultPlayer()
{
}

//=============================================================================
// 更新
//=============================================================================
void ResultPlayer::Update()
{
	UpdateAnim(TIME_PER_FRAME);
}

//=============================================================================
// 描画
//=============================================================================
void ResultPlayer::Draw()
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
// アニメーションのコールバック
//=============================================================================
HRESULT CALLBACK ResultPlayer::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}

//=============================================================================
// アニメーションセットの作成
//=============================================================================
void ResultPlayer::CreateAnimSet()
{
	ANIMATIONSET *AnimationSet = new ANIMATIONSET();
	vector<KEYDATA>Keydata;
	Keydata.reserve(Keydata_Max);

	for (int Set_No = 0; Set_No < GetAnimSetNum(); Set_No++)
	{
		switch (Set_No)
		{
		case Idle:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Idle", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Running:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Running", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Jump:

			//Keydata.push_back(KEYDATA{ 0.80f, e_MotionEnd });
			AnimationSet->SetData("Jump", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Victory:

			AnimationSet->SetData("Victory", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Slip:

			//Keydata.push_back(KEYDATA{ 0.95f, e_MotionChange });
			AnimationSet->SetData("Slip", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Stop:

			AnimationSet->SetData("Stop", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Lose:

			AnimationSet->SetData("Lose", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Clapping:

			AnimationSet->SetData("Clapping", NULL, 1.5f, 0.1f, 0.0f);
			break;

		default:
			break;
		}

		this->SetupCallbackKeys(&Keydata, AnimationSet->GetSetName());

		AnimationSet->SetAnimSetPtr(this->AnimController);

		this->AnimSet.push_back(*AnimationSet);

		Keydata.clear();
	}

	SAFE_DELETE(AnimationSet);
	ReleaseVector(Keydata);

}