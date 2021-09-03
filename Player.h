//=============================================================================
//
// プレイヤークラス[Player.cpp]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "D3DXAnimation.h"
#include "CharacterAI.h"
#include "PlayerUI.h"
#include "PlayerState.h"
#include "FieldItemManager.h"
#include "PaintManager.h"
#include "Map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX				(4)										// 操作するプレイヤーの数
#define PLAYER_COLLISION_SIZE	D3DXVECTOR2(5.0f, 5.0f)					// 当たり判定を有効にするサイズ（足元のみ）
#define JUMP_SPEED				(12.0f)									// ジャンプの初速

// 読み込むキャラクターモデル
static const char* CharaModel[] =
{
	"data/MODEL/Kaya/Kaya.x",
	"data/MODEL/Aj/Aj.x",
	"data/MODEL/Granny/Granny.x",
	"data/MODEL/Claire/Claire.x",
};

// キャラクターモデルの番号
enum CharaModelNum
{
	KayaModel,
	AjModel,
	GrannyModel,
	ClaireModel,

	// モデルの種類
	MaxModel
};

// キャラクターのアニメーション番号と連動（CharaStateAnim）
enum CharaStateNum
{
	Idle,
	Running,
	Jump,
	Victory,
	Slip,
	Stop,
	Lose,
	Clapping,
	AnimMax,			// アニメーションの最大数
};
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Player :
	public D3DXANIMATION
{
private:
	// メンバ変数
	PlayerState			*state;				// ステータス管理抽象クラス
	FieldItemManager	*itemManager;		// フィールドアイテム管理クラス
	PaintManager		*PaintSystem;		// ペイントシステム
	PlayerUI			*playerUI;
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
	int					ctrlNum;			// 操作するコントローラ番号
	float				animSpd;			// アニメーションの再生スピード
	bool				playable;			// 操作可能
	bool				onCamera;			// 画面内にいるとき

	// AI用
	CharacterAI			*AI;				// キャラクターAI
	bool				AIUse = false;

	// ステータス関係
	float				runSpd;				// ダッシュ速度(0.0-1.0-2.0)
	float				jumpSpd;			// ジャンプ速度
	float				jumpValue;			// ジャンプ速度に掛けて使う(0.0-1.0-2.0)

											// 当たり判定関係のフラグ
	bool				hitGround;			// 地上判定(↓と合わせて両方falseだと空中状態)
	bool				hitPaint;
	bool				hitHorizon;			// 進行方向のオブジェクトとの当たり判定
	bool				hitItem;			// アイテムとの当たり判定

											// カウンタ
	int					hitObjCnt;			// オブジェクトにあたったときのカウンタ

											// アイテム関連のステータス
	bool				spike;				// スパイクブーツ装備中
	bool				blind;				// ブラインド中
	bool				jet;				// ジェットパック装備中
	bool				PowerBanana;		// パワーバナナ使用中

	// メンバ関数
	void Move();			// 移動
	void CheckOnCamera();
	void JumpMove();		// ジャンプ移動
	void Debug();			// デバッグ
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet()override;

public:
	// メンバ関数
	Player(int _CtrlNum, bool AIUse);
	~Player();
	void Update();
	void Draw();

	// 状態抽象インターフェース
	void UpdateState(int AnimCurtID);
	void ChangeState(PlayerState *NewState);

	// 当たり判定
	void GroundCollider();
	void HorizonCollider();
	void ObjectCollider();		// フィールドオブジェクト
	void ObjectItemCollider(Map *pMap);	// フィールドオブジェクト（アイテム）
	void PaintCollider();
	void FieldItemCollider(FieldItemManager *pFIManager);	// フィールド内に設置されたアイテム

	void HitObjectInfluence(int type);	// フィールドオブジェクトに接触したときの効果

	// エフェクトの発生
	void PowwrUpEffect();
	void RunningEffect();
	void ItemGetEffect(D3DXVECTOR3 pos);

	// ゲッター(なるべく使わない)
	D3DXVECTOR3 GetPos() { return pos; };
	FieldItemManager *GetFieldItemManager() { return itemManager; };
	float GetJumpSpeed() { return jumpSpd; };
	float GetJumpValue() { return jumpValue; };
	PaintManager* GetPaintManager(void) { return this->PaintSystem; };
	bool GetOnCamera() { return onCamera; };
	int GetCtrlNum() { return ctrlNum; };
	bool GetPlayable() { return playable; };
	bool GetHitGround() { return hitGround; };
	bool GetHitHorizon() { return hitHorizon; };
	bool GetHitPaint() { return hitPaint; };
	bool GetHitItem() { return hitItem; };
	bool GetSpike() { return spike; };
	bool GetBlind() { return blind; };
	bool GetBanana() { return PowerBanana; };
	bool GetJet() { return jet; };

	// AI用
	bool GetAIUse(void) { return this->AIUse; };
	CharacterAI* GetAIPtr(void) { return this->AI; };
	int GetAIAction(void) { return AIUse == true ? AI->GetAIAction() : eNoAction; };

	// セッター
	void SetJumpSpeed(float _JumpSpeed) { jumpSpd = _JumpSpeed; };
	void SetPlayable(bool _playable) { playable = _playable; };
	void SetHitItem(bool _hitItem) { hitItem = _hitItem; };
	void SetOnCamera(bool Flag) { this->onCamera = Flag; };
	void SetJet(bool _jet) { jet = _jet; };
	void SetPowerBanana(bool Flag) { this->PowerBanana = Flag; };
	void SetSpike(bool _spike) { spike = _spike; };
	//void SetGun(bool _gun) { gun = _gun; };
	void SetBlind(bool _blind) { blind = _blind; };
	void SetRunSpd(float _runSpd) { runSpd = _runSpd; };
	void SetJumpValue(float _jumpValue) { jumpValue = _jumpValue; };
	//void SetAnimSpd(float _animSpd) { animSpd = _animSpd; };
};

#endif
