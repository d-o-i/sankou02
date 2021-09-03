//=============================================================================
//
// ペイント管理クラス [PaintManager.h]
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
// マクロ定義
//*****************************************************************************
#define INK_MAX	(50)	// インクの最大量


//*****************************************************************************
// クラス定義
//*****************************************************************************
class PaintManager
{
private:
	std::vector<Paint*>		BlackPaint;
	std::vector<Paint*>		ColorPaint;
	std::vector<InkGauge*>	inkGauge;
	Cursor					*pCursor;				// 参照するカーソルクラスのポインタ
	int						Owner;
	int						InkValue[InkNum];		// インクの残量
	int						InkType;				// 使用するインクの種類(enum ColorInk=カラー, BlackInk=黒)
	bool					SpInk;					// trueのときインクが減らない
	int						HealCnt;				// 自動回復カウント

	// AI用
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

	// ゲッター
	bool GetSpInk() { return SpInk; };
	int GetInkValue(int type) { return InkValue[type]; };
	int GetOwner(void) { return this->Owner; };
	std::vector<Paint*> GetBlackPaint(void) { return this->BlackPaint; };
	std::vector<Paint*> GetColorPaint(void) { return this->ColorPaint; };
	// 四分木から衝突可能なオブジェクトを探す
	std::vector<Paint*> GetCollisionList(int NodeID);

	// セッター
	void SetSpInk(bool _SpInk) { SpInk = _SpInk; };
	void SetInkValue(int value, int type) { InkValue[type] = value; };
};

#endif
