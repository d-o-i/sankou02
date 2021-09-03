//=============================================================================
//
// アイテム処理[Item.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Item.h"
#include "MyLibrary.h"
#include "Input.h"
#include "BanananoKawaState.h"
#include "BlindState.h"
#include "GunState.h"
#include "JetState.h"
#include "PowerUpBananaState.h"
#include "SpikeState.h"
#include "SpInkState.h"
#include "DebugWindow.h"
#include "EffectManager.h"
#include "Sound.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include "MyLibrary.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ItemPos (D3DXVECTOR3(270.0f, 55.0f, 0.0f))
#define ItemInterval (320.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
Item::Item(Player *ptr)
{
	ResourceManager::Instance()->GetTexture("Item", &D3DTexture);

	pPlayer = ptr;
	use = false;
	pos = D3DXVECTOR3(ItemPos.x + pPlayer->GetCtrlNum() * ItemInterval, ItemPos.y, ItemPos.z);
	PatternAnim = 0;
	rouletteCnt = 0;
	useCnt = 0;
	size = D3DXVECTOR3(ITEM_SIZE.x, ITEM_SIZE.y, 0.0f);
	active = false;

	// 頂点情報の作成
	MakeVertex();

	// ステートパターンの初期化
	state[NumJet] = new JetState(this);
	state[NumSpike] = new SpikeState(this);
	state[NumPowerUp] = new PowerUpBananaState(this);
	state[NumKawa] = new BanananoKawaState(this);
	state[NumBlind] = new BlindState(this);
	state[NumSpInk] = new SpInkState(this);
	state[NumGun] = new GunState(this);
}

//=============================================================================
// デストラクタ
//=============================================================================
Item::~Item()
{
	D3DTexture = NULL;
	
	pPlayer = nullptr;

	// ステートパターンの削除
	for (int i = 0; i < NumItemMax; i++)
	{
		delete state[i];
	}

}

//=============================================================================
// 更新
//=============================================================================
void Item::Update()
{
	if (pPlayer->GetOnCamera())
	{
		// アイテムを取得した瞬間の処理
		if (!use && pPlayer->GetHitItem() && !active)
		{
			Start();

			//テクスチャ座標をセット
			SetTexture();
		}

		// 取得している状態
		if (use)
		{
			// エフェクトを発生させる
			std::vector<Effect*> *EffectVector = GetEffectVector();
			effect = new Effect(ExplosionEffect, pos, INFINITY_LOOP);
			EffectVector->push_back(effect);
			use = false;
			active = true;
			state[PatternAnim]->Start();

			PlaySound(SE_USEITEM);
			// アイテムを使用する
			if ((GetKeyboardTrigger(DIK_I) || IsButtonTriggered(pPlayer->GetCtrlNum(), BUTTON_D)) ||
				(pPlayer->GetAIUse() && pPlayer->GetAIPtr()->GetUseItem()))
			{
				// エフェクトを発生させる
				std::vector<Effect*> *EffectVector = GetEffectVector();
				effect = new Effect(ExplosionEffect, D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, 0.0f), INFINITY_LOOP);
				EffectVector->push_back(effect);

				use = false;
				active = true;
				state[PatternAnim]->Start();

				// AIがアイテムを使用したら
				if (pPlayer->GetAIUse())
				{
					pPlayer->GetAIPtr()->SetUseItem(false);
				}

				// PlaySound(アイテム使用)
			}

			//テクスチャ座標をセット
			SetTexture();
		}

		// アイテム使用中
		if (active)
		{
			ActiveState(PatternAnim);
		}

#if _DEBUG
		Debug();
#endif
	}
}

//=============================================================================
// 描画
//=============================================================================
void Item::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use || (!use && pPlayer->GetHitItem()) || active)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT Item::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertex();

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
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void Item::SetTexture()
{
	int x = PatternAnim % DIVIDE_ITEM_X;
	float sizeX = 1.0f / DIVIDE_ITEM_X;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void Item::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// 取得したときの処理
//=============================================================================
void Item::Start()
{
	// ルーレットを回す
	rouletteCnt = LoopCountUp(rouletteCnt, 0, ROULETTE_COUNTER);
	if (rouletteCnt == 0)
	{
		PlaySound(SE_ITEMRU);
		// テクスチャも回す
		PatternAnim = LoopCountUp(PatternAnim, 0, NumItemMax);
		if (PatternAnim == 0)
		{
			useCnt++;
			// 一定回数回したら使えるようにする
			if (useCnt == USE_COUNTER)
			{
				use = true;
				rouletteCnt = 0;
				useCnt = 0;

				// ランダムでアイテムの種類をセット
				PatternAnim = rand() % NumItemMax;

				if (pPlayer->GetAIUse())
				{
					pPlayer->GetAIPtr()->SetHaveItem(true);
					pPlayer->GetAIPtr()->SetItemType(PatternAnim);
				}
			}
		}
	}
}

//=============================================================================
// アイテムを使用中の効果
//=============================================================================
void Item::ActiveState(int ItemID)
{
	state[ItemID]->Update();
}

//=============================================================================
// アイテムが変化したときに呼び出す
//=============================================================================
void Item::ChangeState(int ItemID)
{
	state[ItemID]->Start();
	SetTexture();
	active = false;
	use = true;
	if (pPlayer->GetAIUse())
	{
		pPlayer->GetAIPtr()->SetHaveItem(true);
		pPlayer->GetAIPtr()->SetItemType(PatternAnim);
	}

	// エフェクトも終了
	if (effect != nullptr)
	{
		effect->SetUse(false);
	}
}

//=============================================================================
// アイテムの効果が終了したときに呼び出す
//=============================================================================
void Item::Reset()
{
	pPlayer->SetHitItem(false);
	use = false;
	active = false;

	// エフェクトも終了
	if (effect != nullptr)
	{
		effect->SetUse(false);
	}
}

//=============================================================================
// デバッグ
//=============================================================================
void Item::Debug()
{
	if (GetKeyboardTrigger(DIK_NUMPAD0))
	{
		use = true;
		PatternAnim = 0;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD1))
	{
		use = true;
		PatternAnim = 1;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD2))
	{
		use = true;
		PatternAnim = 2;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD3))
	{
		use = true;
		PatternAnim = 3;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD4))
	{
		use = true;
		PatternAnim = 4;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD5))
	{
		use = true;
		PatternAnim = 5;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}
	else if (GetKeyboardTrigger(DIK_NUMPAD6))
	{
		use = true;
		PatternAnim = 6;
		if (pPlayer->GetAIUse())
		{
			pPlayer->GetAIPtr()->SetHaveItem(true);
			pPlayer->GetAIPtr()->SetItemType(PatternAnim);
		}
	}

#if 0
	ImGui::SetNextWindowPos(ImVec2(5, 145), ImGuiSetCond_Once);

	BeginDebugWindow("Item");

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode((void*)(intptr_t)pPlayer->GetCtrlNum(), "Player %d", pPlayer->GetCtrlNum()))
	{
		DebugText("ItemNum:%d", PatternAnim);

		if (ImGui::TreeNode("Flag"))
		{
			DebugText("Use:%d Active:%d, PlayerHit:%d", use, active, pPlayer->GetHitItem());
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	EndDebugWindow("Item");
#endif

}