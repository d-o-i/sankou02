//=============================================================================
//
// �A�C�e������[Item.cpp]
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
// �}�N����`
//*****************************************************************************
#define ItemPos (D3DXVECTOR3(270.0f, 55.0f, 0.0f))
#define ItemInterval (320.0f)

//=============================================================================
// �R���X�g���N�^
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

	// ���_���̍쐬
	MakeVertex();

	// �X�e�[�g�p�^�[���̏�����
	state[NumJet] = new JetState(this);
	state[NumSpike] = new SpikeState(this);
	state[NumPowerUp] = new PowerUpBananaState(this);
	state[NumKawa] = new BanananoKawaState(this);
	state[NumBlind] = new BlindState(this);
	state[NumSpInk] = new SpInkState(this);
	state[NumGun] = new GunState(this);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Item::~Item()
{
	D3DTexture = NULL;
	
	pPlayer = nullptr;

	// �X�e�[�g�p�^�[���̍폜
	for (int i = 0; i < NumItemMax; i++)
	{
		delete state[i];
	}

}

//=============================================================================
// �X�V
//=============================================================================
void Item::Update()
{
	if (pPlayer->GetOnCamera())
	{
		// �A�C�e�����擾�����u�Ԃ̏���
		if (!use && pPlayer->GetHitItem() && !active)
		{
			Start();

			//�e�N�X�`�����W���Z�b�g
			SetTexture();
		}

		// �擾���Ă�����
		if (use)
		{
			// �G�t�F�N�g�𔭐�������
			std::vector<Effect*> *EffectVector = GetEffectVector();
			effect = new Effect(ExplosionEffect, pos, INFINITY_LOOP);
			EffectVector->push_back(effect);
			use = false;
			active = true;
			state[PatternAnim]->Start();

			PlaySound(SE_USEITEM);
			// �A�C�e�����g�p����
			if ((GetKeyboardTrigger(DIK_I) || IsButtonTriggered(pPlayer->GetCtrlNum(), BUTTON_D)) ||
				(pPlayer->GetAIUse() && pPlayer->GetAIPtr()->GetUseItem()))
			{
				// �G�t�F�N�g�𔭐�������
				std::vector<Effect*> *EffectVector = GetEffectVector();
				effect = new Effect(ExplosionEffect, D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, 0.0f), INFINITY_LOOP);
				EffectVector->push_back(effect);

				use = false;
				active = true;
				state[PatternAnim]->Start();

				// AI���A�C�e�����g�p������
				if (pPlayer->GetAIUse())
				{
					pPlayer->GetAIPtr()->SetUseItem(false);
				}

				// PlaySound(�A�C�e���g�p)
			}

			//�e�N�X�`�����W���Z�b�g
			SetTexture();
		}

		// �A�C�e���g�p��
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
// �`��
//=============================================================================
void Item::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use || (!use && pPlayer->GetHitItem()) || active)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT Item::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex();

	// rhw�̐ݒ�
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void Item::SetTexture()
{
	int x = PatternAnim % DIVIDE_ITEM_X;
	float sizeX = 1.0f / DIVIDE_ITEM_X;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void Item::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// �擾�����Ƃ��̏���
//=============================================================================
void Item::Start()
{
	// ���[���b�g����
	rouletteCnt = LoopCountUp(rouletteCnt, 0, ROULETTE_COUNTER);
	if (rouletteCnt == 0)
	{
		PlaySound(SE_ITEMRU);
		// �e�N�X�`������
		PatternAnim = LoopCountUp(PatternAnim, 0, NumItemMax);
		if (PatternAnim == 0)
		{
			useCnt++;
			// ���񐔉񂵂���g����悤�ɂ���
			if (useCnt == USE_COUNTER)
			{
				use = true;
				rouletteCnt = 0;
				useCnt = 0;

				// �����_���ŃA�C�e���̎�ނ��Z�b�g
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
// �A�C�e�����g�p���̌���
//=============================================================================
void Item::ActiveState(int ItemID)
{
	state[ItemID]->Update();
}

//=============================================================================
// �A�C�e�����ω������Ƃ��ɌĂяo��
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

	// �G�t�F�N�g���I��
	if (effect != nullptr)
	{
		effect->SetUse(false);
	}
}

//=============================================================================
// �A�C�e���̌��ʂ��I�������Ƃ��ɌĂяo��
//=============================================================================
void Item::Reset()
{
	pPlayer->SetHitItem(false);
	use = false;
	active = false;

	// �G�t�F�N�g���I��
	if (effect != nullptr)
	{
		effect->SetUse(false);
	}
}

//=============================================================================
// �f�o�b�O
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