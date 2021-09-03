//=============================================================================
//
// �y�C���g�Ǘ��N���X [PaintManager.h]
// Author :  
//
//=============================================================================
#ifndef _PAINTSYSTEM_H_
#define _PAINTSYSTEM_H_

#include "Paint.h"
#include "PaintGroup.h"
#include "Cursor.h"
#include "Quadtree.h"
#include "InkGauge.h"
#include "CharacterAI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INK_MAX	(50)	// �C���N�̍ő��


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PaintManager
{
private:
	std::vector<Paint*>		BlackPaint;
	std::vector<Paint*>		ColorPaint;
	std::vector<InkGauge*>	inkGauge;
	Cursor					*pCursor;				// �Q�Ƃ���J�[�\���N���X�̃|�C���^
	int						Owner;
	int						InkValue[InkNum];		// �C���N�̎c��
	int						InkType;				// �g�p����C���N�̎��(enum ColorInk=�J���[, BlackInk=��)
	bool					SpInk;					// true�̂Ƃ��C���N������Ȃ�
	int						HealCnt;				// �����񕜃J�E���g

	// AI�p
	bool					PressPaint = false;
	bool					AIUse = false;
	CharacterAI				*AIptr = nullptr;

	static QUADTREE			*Quadtree;
	static PaintGroup		*paintGroup;

	void SetPaint(int InkType);
	void CheckPaintUse(void);
	void AutoHeal();

public:
	PaintManager(int PlayerNo, bool AIUse, CharacterAI *AIptr);
	~PaintManager();

	void Update();
	void Draw();
	static void SetQuadtreePtr(QUADTREE *Quadtree) { if (!PaintManager::Quadtree) { PaintManager::Quadtree = Quadtree; } };
	static void ReleaseQuadtreePtr(void) { PaintManager::Quadtree = nullptr; };
	static void SetPaintGroupPtr(PaintGroup *Ptr) { if (!PaintManager::paintGroup) { PaintManager::paintGroup = Ptr; } };
	static void ReleasePaintGroupPtr(void) { PaintManager::paintGroup = nullptr; };

	// �Q�b�^�[
	bool GetSpInk() { return SpInk; };
	int GetInkValue(int type) { return InkValue[type]; };
	int GetOwner(void) { return this->Owner; };
	std::vector<Paint*> GetBlackPaint(void) { return this->BlackPaint; };
	std::vector<Paint*> GetColorPaint(void) { return this->ColorPaint; };
	// �l���؂���Փˉ\�ȃI�u�W�F�N�g��T��
	std::vector<Paint*> GetCollisionList(int NodeID);

	// �Z�b�^�[
	void SetSpInk(bool _SpInk) { SpInk = _SpInk; };
	void SetInkValue(int value, int type) { InkValue[type] = value; };
};

#endif
