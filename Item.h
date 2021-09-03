//=============================================================================
//
// アイテム処理 [Item.h]
// Author :  
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "_2dobj.h"
#include "Player.h"
#include "ItemState.h"
#include "Effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_SIZE				D3DXVECTOR2(50.0f, 50.0f)
#define ROULETTE_COUNTER		(10)						// 何フレームごとにルーレットを動かすか
#define USE_COUNTER				(3)							// 何回ルーレットを動かしたらアイテムを確定させるか
#define DIVIDE_ITEM_X			(7)
#define DIVIDE_ITEM_Y			(1)

enum ItemNum
{
	NumJet,		// ジェットパック
	NumSpike,	// スパイクブーツ
	NumPowerUp,	// パワーアップバナナ
	NumKawa,	// バナナの皮
	NumBlind,	// ブラインド
	NumSpInk,	// スペシャルインク
	NumGun,		// トリモチガン

	NumItemMax,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Item :
	public _2dobj
{
private:
	Effect *effect;
	ItemState *state[NumItemMax];			// ステータス抽象クラス
	Player *pPlayer;						// 参照するプレイヤークラスのポインタ
	int rouletteCnt;						// ルーレットのカウンタ
	int useCnt;								// ルーレットでアイテムのテクスチャも回すためのカウンタ
	bool active;							// アイテム使用中

	HRESULT MakeVertex();
	void SetTexture();
	void SetVertex();

	void Start();
	void ActiveState(int ItemID);
	void Debug();

public:
	Item(Player* ptr);
	~Item();

	void Update();
	void Draw();

	void ChangeState(int ItemID);
	void Reset();

	bool GetUse() { return use; };
	Player *GetPlayer() { return pPlayer; };
	Effect *GetEffect() { return effect; };

	void SetPatternAnim(int Anim) { PatternAnim = Anim; };
};

#endif
