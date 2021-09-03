//=============================================================================
//
// �y�C���g�Ǘ��N���X [PaintSystem.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "PaintManager.h"
#include "Input.h"
#include "MyLibrary.h"
#include "Camera.h"
#include "Collision.h"
#include "Quadtree.h"
#include "DebugWindow.h"
#include "SceneGame.h"
#include "Player.h"
#include "MyLibrary.h"

#define HEAL_FRAME		(30)	// ���t���[�����ƂɎ����񕜂��s����

QUADTREE *PaintManager::Quadtree = nullptr;
PaintGroup *PaintManager::paintGroup = nullptr;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
PaintManager::PaintManager(int PlayerNo, bool AIUse, CharacterAI *AIptr)
{
	this->Owner = PlayerNo;
	this->InkType = ColorInk;
	if (AIUse)
	{
		this->AIUse = true;
		this->AIptr = AIptr;
		// �J�[�\���I�u�W�F�N�g�쐬
		this->pCursor = new Cursor(this->Owner, true, AIptr);
		CharacterAI::SetPaintGroupPtr(PaintManager::paintGroup);
	}
	else
	{
		this->AIUse = false;
		this->AIptr = nullptr;
		// �J�[�\���I�u�W�F�N�g�쐬
		this->pCursor = new Cursor(this->Owner, false, nullptr);
	}

	// �C���N�c�ʂ��ő�l�ɂ���
	for (int i = 0; i < InkNum; i++)
	{
		this->InkValue[i] = INK_MAX;
	}

	// �C���N�Q�[�WUI�I�u�W�F�N�g�쐬
	this->inkGauge.push_back(new InkGauge(ColorInk, PlayerNo));
	this->inkGauge.push_back(new InkGauge(BlackInk, PlayerNo));

	this->SpInk = false;
	this->HealCnt = 0;

	// �y�C���g�x�N�g���̃������̈�m��
	BlackPaint.reserve(INK_MAX);
	ColorPaint.reserve(INK_MAX);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
PaintManager::~PaintManager()
{
	// �����������[�X
	SAFE_DELETE(this->pCursor);



	for (auto &Object : this->inkGauge)
	{
		SAFE_DELETE(Object);
	}
	this->inkGauge.clear();
	ReleaseVector(inkGauge);

	for (auto &Paint : this->BlackPaint)
	{
		SAFE_DELETE(Paint);
	}
	this->BlackPaint.clear();
	ReleaseVector(BlackPaint);

	for (auto &Paint : this->ColorPaint)
	{
		SAFE_DELETE(Paint);
	}
	this->ColorPaint.clear();
	ReleaseVector(ColorPaint);
}

//=============================================================================
// �X�V����
//=============================================================================
void PaintManager::Update()
{
	// �C���N�̎�����
	AutoHeal();

	// �J�[�\���̍X�V
	this->pCursor->Update();

	// �g�p���Ă��Ȃ��y�C���g���x�N�g������폜
	CheckPaintUse();

	// �g�p���Ă���y�C���g���X�V
	for (auto &Black : this->BlackPaint)
	{
		// �I�u�W�F�N�g����ʓ����A�m�[�h�͈͓̔�����Ȃ��Ȃ�A������x�l���؂ɓ����
		if (Black->GetInScreen() && !PaintManager::Quadtree->CheckObjectInNode(Black))
		{
			PaintManager::Quadtree->InsertObject(Black);
		}
		Black->Update();
	}
	for (auto &Color : this->ColorPaint)
	{
		if (Color->GetInScreen() && !PaintManager::Quadtree->CheckObjectInNode(Color))
		{
			PaintManager::Quadtree->InsertObject(Color);
		}
		Color->Update();
	}

	// �C���N�̎�ތ���
	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(Owner, BUTTON_R1) ||
		(AIUse && AIptr->GetAIChangeInk()))
	{
		InkType = InkType == BlackInk ? ColorInk : BlackInk;
		pCursor->ChangeInk();
		if (AIUse)
		{
			AIptr->SetInkType(InkType);
			AIptr->SetAIChangeInk(false);
		}
	}

	// �C���N���g��
	if (!AIUse)
	{
		// �g�p����C���N�̎c�ʃ`�F�b�N
		if (this->InkValue[InkType] > 0)
		{
			if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(Owner, BUTTON_C))
			{
				PaintManager::paintGroup->Start(Owner);
			}

			if (GetKeyboardPress(DIK_O) || IsButtonPressed(this->Owner, BUTTON_C))
			{
				// �y�C���g��ݒu����
				SetPaint(InkType);
				// �C���N�����炷
				if (!SpInk)
				{
					InkValue[InkType]--;
				}
			}

			if ((GetKeyboardRelease(DIK_O) || IsButtonReleased(Owner, BUTTON_C)) ||
				InkValue[InkType] <= 0)
			{
				PaintManager::paintGroup->End(Owner);
			}
		}
	}
	// AI���C���N���g��
	else
	{
		if (AIptr->GetPaintState() == ePaintStart)
		{
			if (InkType == ColorInk)
			{
				PaintManager::paintGroup->Start(Owner);
			}
			AIptr->SetPaintState(ePainting);

			// �y�C���g��ݒu����
			SetPaint(InkType);
		}
		else if (AIptr->GetPaintState() == ePainting)
		{
			// �g�p����C���N�̎c�ʃ`�F�b�N
			if (this->InkValue[InkType] > 0)
			{
				// �y�C���g��ݒu����
				SetPaint(InkType);
				// �C���N�����炷
				if (!SpInk)
				{
					InkValue[InkType]--;

					// AI�̃C���N�c�ʏ�Ԃ��X�V
					if (InkValue[InkType] >= INK_MAX)
					{
						// ���^��
						AIptr->SetInkState(InkType, eInkValue_Full);
					}
					else if (InkValue[InkType] < INK_MAX && InkValue[InkType] >= INK_MAX / 2)
					{
						// �����ȏ�
						AIptr->SetInkState(InkType, eInkValue_Many);
					}
					else if (InkValue[InkType] < INK_MAX / 2 && InkValue[InkType] >= INK_MAX * 0.15)
					{
						// �����ȉ�
						AIptr->SetInkState(InkType, eInkValue_Less);
					}
					else if (InkValue[InkType] < INK_MAX * 0.15)
					{
						// 15%�ȉ�
						AIptr->SetInkState(InkType, eInkValue_Few);
					}
				}
			}
			else
			{
				// �C���N�̎c�ʑ���Ȃ��A�y�C���g�I��
				AIptr->SetPaintState(ePaintEnd);
			}
		}
		else if (AIptr->GetPaintState() == ePaintEnd)
		{
			// �y�C���g��ݒu����
			//SetPaint(InkType);

			// �J���[�C���N�Ȃ�A���̃y�C���g�O���[�v���L�^����
			if (InkType == ColorInk)
			{
				PaintManager::paintGroup->End(Owner);
			}
			AIptr->SetPaintState(eNoAction);
		}
	}

	// �C���N�Q�[�W���X�V
	inkGauge.at(ColorInk)->Update(InkValue[ColorInk]);
	inkGauge.at(BlackInk)->Update(InkValue[BlackInk]);

#if _DEBUG
	// �C���N�̎c�ʂ𒲐�
	if (GetKeyboardPress(DIK_Z))
	{
		InkValue[InkType] = max(--InkValue[InkType], 0);
	}

	if (GetKeyboardPress(DIK_X))
	{
		InkValue[InkType] = min(++InkValue[InkType], INK_MAX);
	}

	if (GetKeyboardPress(DIK_C))
	{
		InkValue[ColorInk] = INK_MAX;
		InkValue[BlackInk] = INK_MAX;
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		for (auto Paint = this->BlackPaint.begin(); Paint != this->BlackPaint.end(); Paint++)
		{
			(*Paint)->SetUse(false);
			SAFE_DELETE((*Paint));
		}
		this->BlackPaint.clear();

		for (auto Paint = this->ColorPaint.begin(); Paint != this->ColorPaint.end(); Paint++)
		{
			(*Paint)->SetUse(false);
			SAFE_DELETE((*Paint));
		}
		this->ColorPaint.clear();
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void PaintManager::Draw()
{
	// �g�p���Ă���y�C���g��`��
	for (auto &Black : this->BlackPaint)
	{
		Black->Draw();
	}
	for (auto &Color : this->ColorPaint)
	{
		Color->Draw();
	}

	// �J�[�\����`��
	this->pCursor->Draw();

	// �C���N�Q�[�W��`��
	// ���ݎg�p���Ă���C���N�̓J���[�A�J���[�C���N�Q�[�W�͑O
	if (this->InkType == ColorInk)
	{
		inkGauge.at(BlackInk)->Draw();
		inkGauge.at(ColorInk)->Draw();
	}
	// ���ݎg�p���Ă���C���N�͍��A���C���N�Q�[�W�͑O
	else if (this->InkType == BlackInk)
	{
		inkGauge.at(ColorInk)->Draw();
		inkGauge.at(BlackInk)->Draw();
	}
}

//=============================================================================
// �y�C���g�̃Z�b�g
//=============================================================================
void PaintManager::SetPaint(int InkType)
{
	D3DXVECTOR3 CursorScreenPos = pCursor->GetPos();
	D3DXVECTOR3 CursorWorldPos = pCursor->GetWorldPos();

	// ���C���N�̏ꍇ
	if (InkType == BlackInk)
	{
		if (this->BlackPaint.size() >= INK_MAX)
		{
			return;
		}

		Paint *Object = new Paint(this->Owner, BlackInkColor);

		Object->SetPos(CursorWorldPos);
		Object->SetUse(true);

		// �X�N���[�����W��ۑ�����
		Object->SetScreenPos((D3DXVECTOR2)CursorScreenPos);
		// �l���؂ɓ����
		PaintManager::Quadtree->InsertObject(Object);

		// �g�p���Ă���C���N�̐F�ɍ��킹�ĕ\�����ԁA�e�N�X�`�����Z�b�g
		Object->SetTime(DRAW_FRAME_BLACK);

		// Vector�ɓ����
		this->BlackPaint.push_back(Object);
	}
	// �J���[�C���N�̏ꍇ
	else
	{
		if (this->ColorPaint.size() >= INK_MAX)
		{
			return;
		}

		Paint *Object = new Paint(this->Owner, this->Owner);

		Object->SetPos(CursorWorldPos);
		Object->SetUse(true);

		// �X�N���[�����W��ۑ�����
		Object->SetScreenPos((D3DXVECTOR2)CursorScreenPos);
		// �l���؂ɓ����
		PaintManager::Quadtree->InsertObject(Object);

		PaintManager::paintGroup->Insert(CursorWorldPos, this->Owner);

		// �g�p���Ă���C���N�̐F�ɍ��킹�ĕ\�����ԁA�e�N�X�`�����Z�b�g
		Object->SetTime(DRAW_FRAME_COLOR);

		// Vector�ɓ����
		this->ColorPaint.push_back(Object);
	}
}

//=============================================================================
// �g�p���Ă��Ȃ��y�C���g��Vector����폜
//=============================================================================
void PaintManager::CheckPaintUse(void)
{
	for (auto Paint = this->BlackPaint.begin(); Paint != this->BlackPaint.end();)
	{
		if ((*Paint)->GetUse() == false)
		{
			SAFE_DELETE((*Paint));
			Paint = this->BlackPaint.erase(Paint);
		}
		else
		{
			Paint++;
		}
	}

	for (auto Paint = this->ColorPaint.begin(); Paint != this->ColorPaint.end();)
	{
		if ((*Paint)->GetUse() == false)
		{
			SAFE_DELETE((*Paint));
			Paint = this->ColorPaint.erase(Paint);
		}
		else
		{
			Paint++;
		}
	}
}

//=============================================================================
// �l���؂���Փˉ\�ȃI�u�W�F�N�g��T��
//=============================================================================
std::vector<Paint*> PaintManager::GetCollisionList(int NodeID)
{
	return PaintManager::Quadtree->GetObjectsAt(NodeID);
}

//=============================================================================
// �����񕜏���
//=============================================================================
void PaintManager::AutoHeal()
{
	HealCnt = LoopCountUp(HealCnt, 0, HEAL_FRAME);
	if (HealCnt == 0)
	{
		InkValue[ColorInk] = min(++InkValue[ColorInk], INK_MAX);
		InkValue[BlackInk] = min(++InkValue[BlackInk], INK_MAX);

		// AI�̃C���N�c�ʏ�Ԃ��X�V
		if (AIUse)
		{
			for (int i = 0; i < InkNum; i++)
			{
				if (InkValue[i] >= INK_MAX)
				{
					// ���^��
					AIptr->SetInkState(i, eInkValue_Full);
				}
				else if (InkValue[i] < INK_MAX && InkValue[i] >= INK_MAX / 2)
				{
					// �����ȏ�
					AIptr->SetInkState(i, eInkValue_Many);
				}
				else if (InkValue[i] < INK_MAX / 2 && InkValue[i] >= INK_MAX * 0.15)
				{
					// �����ȉ�
					AIptr->SetInkState(i, eInkValue_Less);
				}
				else if (InkValue[i] < INK_MAX * 0.15)
				{
					// 15%�ȉ�
					AIptr->SetInkState(i, eInkValue_Few);
				}
			}
		}
	}
}