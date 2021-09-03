//=============================================================================
//
// CharacterAIクラス[CharacterAI.cpp]
// Author :  
//
//=============================================================================
#ifndef _CharacterAI_H_
#define _CharacterAI_H_

#include "PaintGroup.h"
#include "InkGauge.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 現在AIプレイヤーの状態
enum eAIAction
{
	eActJump,
	eActFallen,
	eNoAction = -1,
};

// 現在AIカーソルの状態
enum eAIState
{
	ePaintPath,			// これからの道をペイントする
	ePaintObjChip,		// オブジェクトチップをペイントする
	eUseBlackPaint,		// 他のプレイヤーのペイントを削除する
};

// 現在AIペイントの状態
enum eAIPaintState
{
	ePaintStart,		// ペイントが始まる
	ePainting,			// ペイント中
	ePaintEnd,			// ペイント終了
};

// 現在AIのインク残量の状態
enum eAIInkState
{
	eInkValue_Full,		// 満タン
	eInkValue_Many,		// 半分以上
	eInkValue_Less,		// 半分以下
	eInkValue_Few,		// 15%以下
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CharacterAI
{
private:
# if _DEBUG
	bool		DrawLineFlag = false;
	void DrawLine3D(D3DXVECTOR3 P1, D3DXVECTOR3 P2);
#endif

	D3DXVECTOR3	PaintStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ペイントの始点
	D3DXVECTOR3	PaintEndPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ペイントの終点
	D3DXVECTOR3 PrePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 前のフレームの座標
	GroupStruct *EnemyPaint;				// 削除するペイント
	int			Owner = 0;					// AIの所有者
	int			Action = eNoAction;			// AIの行動
	int			CursorState = eNoAction;	// カーソル状態
	int			PaintState = eNoAction;		// ペイント状態
	int			InkType = ColorInk;			// インクの種類
	int			InkState[InkNum];			// インク残量の状態
	int			ItemType = 0;				// 持っているアイテムの種類
	int			StopCount = 0;				// 動けないカウント
	bool		ChangeInk = false;			// インクを変更するフラグ
	bool		HaveItem = false;			// アイテムを持っているフラグ
	bool		UseItem = false;			// アイテムを使用するフラグ
	bool		UseSpike = false;			// スパイクブーツを使用するフラグ
	bool		SpecialInk = false;			// スペシャルインクを使用するフラグ
	bool		FindEdgeOver = false;		// プラットフォームのへりを探すフラグ
	bool		FindObjChipOver = false;	// オブジェクトチップを探すフラグ
	bool		RandomOver = false;			// ランダムで行動を決定するフラグ
	bool		FindEnemyPaint = false;		// 削除するペイントを探すフラグ
	bool		ShotBullet = false;			// バレットを撃つフラグ
	static PaintGroup *paintGroup;

	// マップチップの番号によって行動する
	void MapChipAction(D3DXVECTOR3 Pos, int MapChipNo);
	// 画面内の他人のペイントを探して、削除する
	void PaintAction(void);
	// アイテムを持っていたら、アイテムを使用する
	void ItemAction(void);
	// 向こうのプラットフォームを探す
	void FindPlatform(D3DXVECTOR3 Pos);
	// ペイントするオブジェクトチップを探す
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
