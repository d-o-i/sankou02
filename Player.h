//=============================================================================
//
// �v���C���[�N���X[Player.cpp]
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
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX				(4)										// ���삷��v���C���[�̐�
#define PLAYER_COLLISION_SIZE	D3DXVECTOR2(5.0f, 5.0f)					// �����蔻���L���ɂ���T�C�Y�i�����̂݁j
#define JUMP_SPEED				(12.0f)									// �W�����v�̏���

// �ǂݍ��ރL�����N�^�[���f��
static const char* CharaModel[] =
{
	"data/MODEL/Kaya/Kaya.x",
	"data/MODEL/Aj/Aj.x",
	"data/MODEL/Granny/Granny.x",
	"data/MODEL/Claire/Claire.x",
};

// �L�����N�^�[���f���̔ԍ�
enum CharaModelNum
{
	KayaModel,
	AjModel,
	GrannyModel,
	ClaireModel,

	// ���f���̎��
	MaxModel
};

// �L�����N�^�[�̃A�j���[�V�����ԍ��ƘA���iCharaStateAnim�j
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
	AnimMax,			// �A�j���[�V�����̍ő吔
};
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Player :
	public D3DXANIMATION
{
private:
	// �����o�ϐ�
	PlayerState			*state;				// �X�e�[�^�X�Ǘ����ۃN���X
	FieldItemManager	*itemManager;		// �t�B�[���h�A�C�e���Ǘ��N���X
	PaintManager		*PaintSystem;		// �y�C���g�V�X�e��
	PlayerUI			*playerUI;
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					ctrlNum;			// ���삷��R���g���[���ԍ�
	float				animSpd;			// �A�j���[�V�����̍Đ��X�s�[�h
	bool				playable;			// ����\
	bool				onCamera;			// ��ʓ��ɂ���Ƃ�

	// AI�p
	CharacterAI			*AI;				// �L�����N�^�[AI
	bool				AIUse = false;

	// �X�e�[�^�X�֌W
	float				runSpd;				// �_�b�V�����x(0.0-1.0-2.0)
	float				jumpSpd;			// �W�����v���x
	float				jumpValue;			// �W�����v���x�Ɋ|���Ďg��(0.0-1.0-2.0)

											// �����蔻��֌W�̃t���O
	bool				hitGround;			// �n�㔻��(���ƍ��킹�ė���false���Ƌ󒆏��)
	bool				hitPaint;
	bool				hitHorizon;			// �i�s�����̃I�u�W�F�N�g�Ƃ̓����蔻��
	bool				hitItem;			// �A�C�e���Ƃ̓����蔻��

											// �J�E���^
	int					hitObjCnt;			// �I�u�W�F�N�g�ɂ��������Ƃ��̃J�E���^

											// �A�C�e���֘A�̃X�e�[�^�X
	bool				spike;				// �X�p�C�N�u�[�c������
	bool				blind;				// �u���C���h��
	bool				jet;				// �W�F�b�g�p�b�N������
	bool				PowerBanana;		// �p���[�o�i�i�g�p��

	// �����o�֐�
	void Move();			// �ړ�
	void CheckOnCamera();
	void JumpMove();		// �W�����v�ړ�
	void Debug();			// �f�o�b�O
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet()override;

public:
	// �����o�֐�
	Player(int _CtrlNum, bool AIUse);
	~Player();
	void Update();
	void Draw();

	// ��Ԓ��ۃC���^�[�t�F�[�X
	void UpdateState(int AnimCurtID);
	void ChangeState(PlayerState *NewState);

	// �����蔻��
	void GroundCollider();
	void HorizonCollider();
	void ObjectCollider();		// �t�B�[���h�I�u�W�F�N�g
	void ObjectItemCollider(Map *pMap);	// �t�B�[���h�I�u�W�F�N�g�i�A�C�e���j
	void PaintCollider();
	void FieldItemCollider(FieldItemManager *pFIManager);	// �t�B�[���h���ɐݒu���ꂽ�A�C�e��

	void HitObjectInfluence(int type);	// �t�B�[���h�I�u�W�F�N�g�ɐڐG�����Ƃ��̌���

	// �G�t�F�N�g�̔���
	void PowwrUpEffect();
	void RunningEffect();
	void ItemGetEffect(D3DXVECTOR3 pos);

	// �Q�b�^�[(�Ȃ�ׂ��g��Ȃ�)
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

	// AI�p
	bool GetAIUse(void) { return this->AIUse; };
	CharacterAI* GetAIPtr(void) { return this->AI; };
	int GetAIAction(void) { return AIUse == true ? AI->GetAIAction() : eNoAction; };

	// �Z�b�^�[
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
