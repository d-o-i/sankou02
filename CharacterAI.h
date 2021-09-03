//=============================================================================
//
// CharacterAI�N���X[CharacterAI.cpp]
// Author :  
//
//=============================================================================
#ifndef _CharacterAI_H_
#define _CharacterAI_H_

#include "PaintGroup.h"
#include "InkGauge.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
// ����AI�v���C���[�̏��
enum eAIAction
{
	eActJump,
	eActFallen,
	eNoAction = -1,
};

// ����AI�J�[�\���̏��
enum eAIState
{
	ePaintPath,			// ���ꂩ��̓����y�C���g����
	ePaintObjChip,		// �I�u�W�F�N�g�`�b�v���y�C���g����
	eUseBlackPaint,		// ���̃v���C���[�̃y�C���g���폜����
};

// ����AI�y�C���g�̏��
enum eAIPaintState
{
	ePaintStart,		// �y�C���g���n�܂�
	ePainting,			// �y�C���g��
	ePaintEnd,			// �y�C���g�I��
};

// ����AI�̃C���N�c�ʂ̏��
enum eAIInkState
{
	eInkValue_Full,		// ���^��
	eInkValue_Many,		// �����ȏ�
	eInkValue_Less,		// �����ȉ�
	eInkValue_Few,		// 15%�ȉ�
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CharacterAI
{
private:
# if _DEBUG
	bool		DrawLineFlag = false;
	void DrawLine3D(D3DXVECTOR3 P1, D3DXVECTOR3 P2);
#endif

	D3DXVECTOR3	PaintStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �y�C���g�̎n�_
	D3DXVECTOR3	PaintEndPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �y�C���g�̏I�_
	D3DXVECTOR3 PrePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �O�̃t���[���̍��W
	GroupStruct *EnemyPaint;				// �폜����y�C���g
	int			Owner = 0;					// AI�̏��L��
	int			Action = eNoAction;			// AI�̍s��
	int			CursorState = eNoAction;	// �J�[�\�����
	int			PaintState = eNoAction;		// �y�C���g���
	int			InkType = ColorInk;			// �C���N�̎��
	int			InkState[InkNum];			// �C���N�c�ʂ̏��
	int			ItemType = 0;				// �����Ă���A�C�e���̎��
	int			StopCount = 0;				// �����Ȃ��J�E���g
	bool		ChangeInk = false;			// �C���N��ύX����t���O
	bool		HaveItem = false;			// �A�C�e���������Ă���t���O
	bool		UseItem = false;			// �A�C�e�����g�p����t���O
	bool		UseSpike = false;			// �X�p�C�N�u�[�c���g�p����t���O
	bool		SpecialInk = false;			// �X�y�V�����C���N���g�p����t���O
	bool		FindEdgeOver = false;		// �v���b�g�t�H�[���̂ւ��T���t���O
	bool		FindObjChipOver = false;	// �I�u�W�F�N�g�`�b�v��T���t���O
	bool		RandomOver = false;			// �����_���ōs�������肷��t���O
	bool		FindEnemyPaint = false;		// �폜����y�C���g��T���t���O
	bool		ShotBullet = false;			// �o���b�g�����t���O
	static PaintGroup *paintGroup;

	// �}�b�v�`�b�v�̔ԍ��ɂ���čs������
	void MapChipAction(D3DXVECTOR3 Pos, int MapChipNo);
	// ��ʓ��̑��l�̃y�C���g��T���āA�폜����
	void PaintAction(void);
	// �A�C�e���������Ă�����A�A�C�e�����g�p����
	void ItemAction(void);
	// �������̃v���b�g�t�H�[����T��
	void FindPlatform(D3DXVECTOR3 Pos);
	// �y�C���g����I�u�W�F�N�g�`�b�v��T��
	void FindObjectChip(D3DXVECTOR3 Pos);

public:
	CharacterAI(int Owner);
	~CharacterAI();
	void Update(D3DXVECTOR3 Pos);
# if _DEBUG
	void Draw(void);
#endif

	void SetAIAction(int eAction) { this->Action = eAction; };
	void SetCursorState(int eState) { this->CursorState = eState; };
	void SetPaintState(int State) { this->PaintState = State; };
	void SetInkType(int InkType) { this->InkType = InkType; };
	void SetInkState(int InkType, int InkState) { this->InkState[InkType] = InkState; };
	void SetItemType(int ItemType) { this->ItemType = ItemType; };
	void SetAIChangeInk(bool Flag) { this->ChangeInk = Flag; };
	void SetHaveItem(bool Flag) { this->HaveItem = Flag; };
	void SetUseItem(bool Flag) { this->UseItem = Flag; };
	void SetFindEnemyPaint(bool Flag) { this->FindEnemyPaint = Flag; };
	void SetShotBullet(bool Flag) { this->ShotBullet = Flag; };
	static void SetPaintGroupPtr(PaintGroup *Ptr) { if (!CharacterAI::paintGroup) { CharacterAI::paintGroup = Ptr; } };
	static void ReleasePaintGroupPtr(void) { CharacterAI::paintGroup = nullptr; };

	int GetAIAction(void) { return this->Action; };
	int GetCursorState(void) { return this->CursorState; };
	int GetPaintState(void) { return this->PaintState; };
	bool GetAIChangeInk(void) { return this->ChangeInk; };
	bool GetHaveItem(void) { return this->HaveItem; };
	bool GetUseItem(void) { return this->UseItem; };
	D3DXVECTOR3 GetPaintStartPos(void) { return this->PaintStartPos; };
	D3DXVECTOR3 GetPaintEndPos(void) { return this->PaintEndPos; };
	GroupStruct* GetEnemyPaint(void) { return this->EnemyPaint; };
};

#endif
