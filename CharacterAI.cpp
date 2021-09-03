//=============================================================================
//
// CharacterAI�N���X[CharacterAI.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "CharacterAI.h"
#include "Map.h"
#include "Item.h"
#include "SceneGame.h"
#include "Input.h"
#include "DebugWindow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
PaintGroup *CharacterAI::paintGroup = nullptr;

#define ScanRange_Plat (10)		// �v���b�g�t�H�[���̂ւ��T����͈�
#define ScanRange_ObjChip (3)	// �I�u�W�F�N�g�`�b�v��T����͈�

// CSV���̔ԍ��̎���
enum e_ChipEvent
{
	eNoEventChip = -1,
	eRandomChip = -2,		// �y�C���g�A�����̂ǂ��炩�����_���Ō��߂�
	eJumpChip = -3,			// �W�����v����
	ePaintChip = -4,		// �y�C���g����
	ePlatformEdge = -5,		// �������̃v���b�g�t�H�[���̂ւ�
	ePlatformEdge_Item = -6,// �v���b�g�t�H�[���̂ւ�(�A�C�e������)
	ePaintObject = -7,		// �O���̃I�u�W�F�N�g�`�b�v���y�C���g����
};

#if _DEBUG
// 3D�������_�t�H�[�}�b�g( ���_���W[3D] / ���ˌ� )
#define	FVF_LINE_3D		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

// 3D��ԂŒ����`��p�\���̂��`
typedef struct
{
	D3DXVECTOR3 Point;
	D3DCOLOR	Color;
} VERTEX_3DLINE;
#endif

//=====================================================================================================
// �R���X�g���N�^
//=====================================================================================================
CharacterAI::CharacterAI(int Owner)
{
	this->Owner = Owner;
	this->InkState[ColorInk] = eInkValue_Full;
	this->InkState[BlackInk] = eInkValue_Full;
}

//=====================================================================================================
// �f�X�g���N�^
//=====================================================================================================
CharacterAI::~CharacterAI()
{
	CharacterAI::paintGroup = nullptr;
}

//=====================================================================================================
// �X�V����
//=====================================================================================================
void CharacterAI::Update(D3DXVECTOR3 Pos)
{
	int MapChipNo = Map::GetMapTbl(Pos, eCenterUp);
	this->Action = eNoAction;
#if _DEBUG
	this->DrawLineFlag = false;
#endif

	// �b���v���C���[�������Ȃ��Ȃ�
	if (Pos == PrePos)
	{
		StopCount++;
		if (StopCount >= 30)
		{
			// �W�����v����
			this->Action = eActJump;
			StopCount = 0;
		}
	}
	else
	{
		StopCount = 0;
	}


	// �}�b�v�`�b�v�̔ԍ��ɂ���čs������
	MapChipAction(Pos, MapChipNo);

	// ��ʓ��̑��l�̃y�C���g��T���āA�폜����
	if (CursorState != ePaintPath)
	{
		PaintAction();
	}

	// �A�C�e���������Ă�����A�A�C�e�����g�p����
	if (HaveItem)
	{
		ItemAction();
	}

	// 1�t���[���O�̍��W��ۑ�����
	PrePos = Pos;

#if _DEBUG
	ImGui::SetNextWindowPos(ImVec2(5, 330), ImGuiSetCond_Once);

	BeginDebugWindow("AI");

	DebugText("Action : %d", this->Action);
	DebugText("State : %d", this->CursorState);

	EndDebugWindow("AI");
#endif
}

//=====================================================================================================
// �}�b�v�`�b�v�̔ԍ��ɂ���čs������
//=====================================================================================================
void CharacterAI::MapChipAction(D3DXVECTOR3 Pos, int MapChipNo)
{
	// �t���O�̏�����
	if (MapChipNo == eNoEventChip)
	{
		RandomOver = false;
		FindEdgeOver = false;
		FindObjChipOver = false;
	}

	if (MapChipNo == eRandomChip && !RandomOver)
	{
		int Random = rand() % (2 + 1);

		// ����
		if (Random == 0)
		{
			;	// �������Ȃ�
		}
		// �y�C���g
		else
		{
			// �v���b�g�t�H�[���̂ւ��T��
			this->FindPlatform(Pos);
		}

		RandomOver = true;
	}
	else if (MapChipNo == eJumpChip)
	{
		// �W�����v����
		this->Action = eActJump;
	}
	else if (MapChipNo == ePaintChip)
	{
		// �v���b�g�t�H�[���̂ւ��T��
		this->FindPlatform(Pos);
	}
	else if (MapChipNo == ePaintObject && CursorState != eUseBlackPaint)
	{
		// �y�C���g����I�u�W�F�N�g�`�b�v��T��
		this->FindObjectChip(Pos);
	}
}

//=====================================================================================================
// ���̃v���C���[�̃y�C���g��T���āA�폜����
//=====================================================================================================
void CharacterAI::PaintAction(void)
{
	bool Trigger = false;
	static int Count = 0;
	static int Bonus = 0;

	if (!FindEnemyPaint)
	{
		if (!SpecialInk)
		{
			Count++;
		}
		else
		{
			// �X�y�V�����C���N���g�p������Ȃ�A���葬�x�������Ȃ�
			Count += 2;
		}

		// 2�b���Ƃɔ��肷��
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 6)
			{
				Trigger = true;
				Bonus = 0;
				Count = 0;
			}
			else
			{
				// �g�p���Ȃ����A���Ԃɂ���Ďg�p����m���������Ȃ�
				Bonus++;
			}
		}

		if (Trigger)
		{
			// ��ʒ��̃y�C���g���擾����
			if (CharacterAI::paintGroup->GetEnemyPaint(&EnemyPaint, Owner))
			{
				CursorState = eUseBlackPaint;
				if (InkType != BlackInk)
				{
					ChangeInk = true;
					InkType = BlackInk;
				}
				FindEnemyPaint = true;
			}
		}
	}
}

//=====================================================================================================
// �A�C�e�����g�p����
//=====================================================================================================
void CharacterAI::ItemAction(void)
{
	bool ItemTrigger = false;
	static int Count = 0;
	static int Bonus = 0;

	switch (ItemType)
	{
		// �W�F�b�g�p�b�N
	case NumJet:
		// �p���[�A�b�v�o�i�i
	case NumPowerUp:
		// �X�y�V�����C���N
	case NumSpInk:
		// �u���C���h
	case NumBlind:

		Count++;
		// 2�b���Ƃɔ��肷��
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 5)
			{
				// �A�C�e�����g�p����
				ItemTrigger = true;
			}
			else
			{
				// �g�p���Ȃ����A���Ԃɂ���Ďg�p����m���������Ȃ�
				Bonus++;
			}
		}
		break;

		// �X�p�C�N�u�[�c
	case NumSpike:

		Count++;
		// 2�b���Ƃɔ��肷��
		if (Count % 120 == 0)
		{
			int Rand = rand() % (10 + 1);
			if (Rand + Bonus > 5)
			{
				// �A�C�e�����g�p����
				ItemTrigger = true;
			}
			else
			{
				// �g�p���Ȃ����A���Ԃɂ���Ďg�p����m���������Ȃ�
				Bonus++;
			}
		}

		if (UseSpike)
		{
			// �A�C�e�����g�p����
			ItemTrigger = true;
		}
		break;

		// �o�i�i�̔�
	case NumKawa:

		// �ŉ��ʂ���Ȃ��āA���ɑ��̃v���C���[������Ȃ�o�i�i�̔���g��
		if (SceneGame::GetTheLastPlayer() != Owner)
		{
			// �A�C�e�����g�p����
			ItemTrigger = true;
		}
		else
		{
			Count++;
			// 2�b���Ƃɔ��肷��
			if (Count % 120 == 0)
			{
				int Rand = rand() % (10 + 1);
				if (Rand + Bonus > 5)
				{
					// �A�C�e�����g�p����
					ItemTrigger = true;
				}
				else
				{
					// �g�p���Ȃ����A���Ԃɂ���Ďg�p����m���������Ȃ�
					Bonus++;
				}
			}
		}

		break;

		// �g�����`�K��
	case NumGun:

		Count++;
		// 0.5�b���Ƃɔ��肷��
		if (Count % 30 == 0)
		{
			if (ShotBullet || Bonus >= 10)
			{
				// �A�C�e�����g�p����
				ItemTrigger = true;
				ShotBullet = false;
			}
			else
			{
				// �g�p���Ȃ����A���Ԃɂ���Ďg�p����m���������Ȃ�
				Bonus++;
			}
		}
		break;

	default:
		break;
	}

	// �A�C�e�����g�p����
	if (ItemTrigger)
	{
		HaveItem = false;
		UseItem = true;
		Count = 0;
		Bonus = 0;

		// �X�y�V�����C���N���g�p��
		if (ItemType == NumSpInk)
		{
			SpecialInk = true;
		}
	}
}

//=====================================================================================================
// �������̃v���b�g�t�H�[����T��
//=====================================================================================================
void CharacterAI::FindPlatform(D3DXVECTOR3 Pos)
{
	int PlayerChip_X = 0;
	int PlayerChip_Y = 0;
	Map::GetMapChipXY(Pos, &PlayerChip_X, &PlayerChip_Y);
	std::vector<D3DXVECTOR3> TempPos;
	bool Trigger = false;

	if (FindObjChipOver || (InkState[ColorInk] == eInkValue_Few && !SpecialInk))
	{
		return;
	}

	// �X�L�����ł��鉡���͈̔�
	for (int i = PlayerChip_X + 1; i <= PlayerChip_X + ScanRange_Plat; i++)
	{
		// �c���͈̔�
		for (int j = 0; j < MAP_SIZE_Y; j++)
		{
			int k = Map::GetMapTbl(i, j);
			if (k == ePlatformEdge || k == ePlatformEdge_Item)
			{
				// �T�����v���b�g�t�H�[���̍��W
				TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));

				// �������ɃA�C�e������������A�s���m���������Ȃ�
				if (k == ePlatformEdge_Item)
				{
					TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));
					TempPos.push_back(Map::GetMapChipPos(i, j + 1, eLeftCenter));
				}

				Trigger = true;
			}
		}
	}

	if (Trigger)
	{
		// �L�����N�^�[���̃`�b�v�̍��W(�y�C���g�̎n�_)
		PaintStartPos = Map::GetMapChipPos(PlayerChip_X + 1, PlayerChip_Y, eRightUp);
		// �J�[�\����Ԃ�ς��
		CursorState = ePaintPath;
		// �C���N�����i�K�v������Ȃ�j
		if (InkType != ColorInk)
		{
			ChangeInk = true;
			InkType = ColorInk;
		}
		FindEdgeOver = true;

#if _DEBUG
		DrawLineFlag = true;
#endif

		// ���������̃v���b�g�t�H�[��������΁A�����_���Ō��߂�
		if (TempPos.size() > 1)
		{
			int Random = rand() % TempPos.size();
			PaintEndPos = TempPos.at(Random);
		}
		else if (TempPos.size() == 1)
		{
			PaintEndPos = TempPos.at(0);
		}
	}
}

//=====================================================================================================
// �y�C���g����I�u�W�F�N�g�`�b�v��T��
//=====================================================================================================
void CharacterAI::FindObjectChip(D3DXVECTOR3 PlayerPos)
{
	int Chip_X = 0;
	int Chip_Y = 0;
	// �v���C���[�̍��W�ɂ���āA�`�b�v��XY���W���擾
	Map::GetMapChipXY(PlayerPos, &Chip_X, &Chip_Y);
	bool FindStartChip = false;
	int ChipType = 0;

	if (FindObjChipOver || (InkState[BlackInk] == eInkValue_Few && !SpecialInk) || UseSpike)
	{
		return;
	}

	// �ŏ��̃I�u�W�F�N�g�`�b�v�̍��W��T��
	for (int i = 0; i < ScanRange_ObjChip; i++)
	{
		int TempChip_X = Chip_X + 1 + i;
		for (int j = 0; j < ScanRange_ObjChip; j++)
		{
			int TempChip_Y = Chip_Y - (ScanRange_ObjChip / 2) + j;
			// �I�u�W�F�N�g�`�b�v�e�[�u������A�`�b�v��ނ��擾
			ChipType = Map::GetObjTbl(TempChip_X, TempChip_Y);
			if (ChipType == eObjSpddown || ChipType == eObjNuma || ChipType == eObjDrain)
			{
				// �X�p�C�N�u�[�c�������Ă���Ȃ�A��Ɏg��
				if (HaveItem && ItemType == NumSpike)
				{
					UseSpike = true;
					return;
				}
				else
				{
					// �ŏ��̃I�u�W�F�N�g�`�b�v�̍��W
					PaintStartPos = Map::GetMapChipPos(TempChip_X, TempChip_Y, eCenter);
					// �y�C���g�̏I�����W��ۑ�
					PaintEndPos = Map::GetMapChipPos(TempChip_X, TempChip_Y, eCenter);
					// �ŏ��̃I�u�W�F�N�g�`�b�v��T����
					FindStartChip = true;
				}
				break;
			}
		}

		if (FindStartChip)
		{
			break;
		}
	}

	// �y�C���g����I�u�W�F�N�g�`�b�v�̏I�_��T��
	if (FindStartChip)
	{
		//  ��
		// ����			���FPaintStartPos(�N�_)
		//  ��			���F�I�u�W�F�N�g�`�b�v��T���͈�
		Map::GetMapChipXY(PaintStartPos, &Chip_X, &Chip_Y);
		Chip_X++;

		while (true)
		{
			int Count = 0;

			for (int j = 0; j < 3; j++)
			{
				int TempChip_Y = Chip_Y - 1 + j;
				// �I�u�W�F�N�g�`�b�v�e�[�u������A�`�b�v��ނ��擾
				int TempChipNo = Map::GetObjTbl(Chip_X, TempChip_Y);
				// �T�����`�b�v�ƍŏ��̃`�b�v�͓������
				if (ChipType == TempChipNo)
				{
					// �y�C���g�̏I�_���W��ۑ�
					PaintEndPos = Map::GetMapChipPos(Chip_X, TempChip_Y, eCenter);
					Chip_X++;
					Chip_Y = TempChip_Y;
					break;
				}
				else
				{
					Count++;
				}
			}

			// ���[�v�I��
			if (Count >= 3)
			{
				CursorState = ePaintObjChip;
				// �C���N�����i�K�v������Ȃ�j
				if (InkType != BlackInk)
				{
					ChangeInk = true;
					InkType = BlackInk;
				}
				break;
			}
		}
	}
}

#if _DEBUG
//=============================================================================
// �`�揈��
//=============================================================================
void CharacterAI::Draw(void)
{
	if (this->DrawLineFlag)
	{
		//DrawLine3D(this->PaintStartPos, this->PaintEndPos);
	}
}

//=============================================================================
// 3D�����`��
//=============================================================================
void CharacterAI::DrawLine3D(D3DXVECTOR3 P1, D3DXVECTOR3 P2)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX WorldMatrix;
	LPDIRECT3DVERTEXBUFFER9 LineVtxBuffer;		// �����`��p���_�o�b�t�@
	VERTEX_3DLINE *pVtx;

	// �����`��p���_�o�b�t�@�𐶐�
	Device->CreateVertexBuffer(
		sizeof(VERTEX_3DLINE) * 2,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
		FVF_LINE_3D,					// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&LineVtxBuffer,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);							// NULL�ɐݒ�

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	LineVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].Point = P1;
	pVtx[1].Point = P2;

	// �F�̐ݒ�
	pVtx[0].Color = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[1].Color = D3DCOLOR_RGBA(0, 0, 0, 255);

	// ���_�f�[�^���A�����b�N����
	LineVtxBuffer->Unlock();

	// ���C���e�B���O�𖳌��ɂ���
	Device->SetRenderState(D3DRS_LIGHTING, false);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMatrix);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetStreamSource(0, LineVtxBuffer, 0, sizeof(VERTEX_3DLINE));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_LINE_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, NULL);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ���C���e�B���O��L���ɂ���
	Device->SetRenderState(D3DRS_LIGHTING, true);

	SAFE_RELEASE(LineVtxBuffer);

	return;
}

#endif
