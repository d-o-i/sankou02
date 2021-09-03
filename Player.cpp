//=============================================================================
//
// �v���C���[�N���X[Player.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Player.h"
#include "Input.h"
#include "SceneGame.h"
#include "Camera.h"
#include "DebugWindow.h"
#include "Map.h"
#include "Collision.h"
#include "PaintManager.h"
#include "IdleState.h"
#include "MyLibrary.h"
#include "JumpState.h"
#include "StopState.h"
#include "SlipState.h"
#include "Item.h"
#include "Sound.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "SceneCharacterSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CHARA_XFILE			"data/MODEL/Kouhai.x"						// �ǂݍ��ރ��f����(�t�@�C���p�X��)
#define PLAYER_ROT			D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f)	// �����̌���
#define PLAYER_SCL			D3DXVECTOR3(1.0f, 1.0f, 1.0f)
#define MOVE_SPEED			(2.0f)										// �����X�s�[�h
#define DefaultPosition		D3DXVECTOR3(145.0f, 0.0f, 0.0f)				// �v���C���[�����ʒu
//#define DefaultPosition		D3DXVECTOR3(50.0f, 0.0f, 0.0f)				// �v���C���[�����ʒu
// ���ɒ������K�v�����Ȃ�
#define OBJECT_HIT_COUNTER	(5)											// �I�u�W�F�N�g�Ƀq�b�g�����Ƃ��L���ɂȂ�܂ł̃t���[����
#define MOVE_SPEED			(2.0f)										// �����X�s�[�h
#define FALL_VELOCITY_MAX	(20.0f)										// �ő�̗������x
#define STANDARD_GRAVITY	(0.98f)										// �d�͉����x
#define OBJECT_HIT_SIZE		D3DXVECTOR2(20.0f, 60.0f)					// �����蔻����擾����T�C�Y
#define JETPACK_VALUE		(1.5f)										// �W�F�b�g�p�b�N�������̏㏸�l

enum CallbackKeyType
{
	e_NoEvent = 0,
	e_MotionEnd,				// ���[�V�����I��
	e_MotionChange,				// ���[�V������ύX����
};

//=====================================================================================================
// �R���X�g���N�^
//=====================================================================================================
Player::Player(int _CtrlNum, bool AIUse) : state(nullptr)
{
	// xFile��ǂݍ���
	this->Load_xFile(CharaModel[SceneCharacterSelect::GetSelectCharacter(_CtrlNum)], "Player");

	// �A�j���[�V�����Z�b�g��ݒu����
	this->CreateAnimSet();

	// ���݂̃A�j���[�V�������A�C�h����ԂƂ���
	this->ChangeAnim(Idle);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = DefaultPosition - D3DXVECTOR3(15.0f * _CtrlNum, 0.0f, 0.0f);
	rot = PLAYER_ROT;
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	hitGround = false;
	hitPaint = false;
	runSpd = 1.0f;
	jumpSpd = 0.0f;
	ctrlNum = _CtrlNum;
	if (AIUse)
	{
		this->AI = new CharacterAI(ctrlNum);
		this->AIUse = true;
		this->PaintSystem = new PaintManager(ctrlNum, true, this->AI);
	}
	else
	{
		this->AI = nullptr;
		this->AIUse = false;
		this->PaintSystem = new PaintManager(ctrlNum, false, nullptr);
	}
	this->playerUI = new PlayerUI(ctrlNum);
	hitHorizon = false;
	playable = false;
	onCamera = true;
	hitItem = false;
	//animSpd = 1.0f;
	hitObjCnt = 0;
	jumpValue = 1.0f;

	spike = false;
	//gun = false;
	blind = false;

	// �ҋ@��Ԃ���X�^�[�g����
	state = new IdleState(this);

	// �t�B�[���h��̃A�C�e���Ǘ��N���X������
	itemManager = new FieldItemManager();
}

//=====================================================================================================
// �f�X�g���N�^
//=====================================================================================================
Player::~Player()
{
	SAFE_DELETE(this->AI);
	SAFE_DELETE(this->PaintSystem);
	SAFE_DELETE(this->state);
	SAFE_DELETE(this->itemManager);
	SAFE_DELETE(this->playerUI);
}

//=====================================================================================================
// �X�V
//=====================================================================================================
void Player::Update()
{
	if (onCamera)
	{
		// �ړ�
		Move();

		// AI�̍X�V����
		if (AIUse)
		{
			AI->Update(this->pos);
		}

		// �y�C���g�V�X�e���̍X�V����
		PaintSystem->Update();

		// �v���C���[UI�̍X�V����
		playerUI->Update(this->pos);

		// �A�j���[�V�������X�V
		UpdateAnim(TIME_PER_FRAME * animSpd);

		// ��Ԓ��ۃC���^�[�t�F�[�X�̍X�V
		UpdateState(this->GetAnimCurtID());

		// �J����������
		CheckOnCamera();

		// �t�B�[���h��ɐ��������A�C�e���̍X�V
		itemManager->Update();
	}

	// �f�o�b�O�\��������
	Debug();
}

//=====================================================================================================
// �`��
//=====================================================================================================
void Player::Draw()
{
	if (onCamera)
	{
		if (!blind)
		{
			LPDIRECT3DDEVICE9 pDevice = GetDevice();
			D3DMATERIAL9 matDef;
			D3DXMATRIX WorldMtx, SclMtx, RotMtx, TransMtx;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&WorldMtx);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&SclMtx, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �����_�����O
			this->DrawAnim(&WorldMtx);

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}

		// �y�C���g�̕`��
		PaintSystem->Draw();

		// �v���C���[UI�̕`��
		playerUI->Draw(onCamera, blind);

	}
	// �v���C���[���S��UI
	else
	{
		// �v���C���[UI�̕`��()
		playerUI->Draw(onCamera, blind);
	}

	// �t�B�[���h��ɐ��������A�C�e���̕`��
	itemManager->Draw();

#if _DEBUG
	if (this->AI != nullptr)
	{
		AI->Draw();
	}
#endif
}

//=====================================================================================================
// ��Ԓ��ۃC���^�[�t�F�[�X�̍X�V
//=====================================================================================================
void Player::UpdateState(int AnimCurtID)
{
	state->Update(AnimCurtID);
}

//=====================================================================================================
// ��Ԓ��ۃC���^�[�t�F�[�X�̕ύX
//=====================================================================================================
void Player::ChangeState(PlayerState *NewState)
{
	delete state;
	state = NewState;
}

//=====================================================================================================
// �ړ�
//=====================================================================================================
void Player::Move()
{
	if (onCamera)
	{
		// �I�[�g�ړ�
		if (!hitHorizon && playable && pos.x < GOAL_POS.x && GetAnimCurtID() != Slip && GetAnimCurtID() != Stop)
		{
			if (!PowerBanana)
			{
				pos.x += MOVE_SPEED * runSpd;
			}
			else
			{
				pos.x += MOVE_SPEED * 1.3f;
			}
		}
	}

	// �󒆔���
	JumpMove();
}

//=====================================================================================================
// �W�����v�ړ�
//=====================================================================================================
void Player::JumpMove()
{
	pos.y += jumpSpd;
	// �����ő呬�x�����x���ꍇ�A�������x���d�͉����x�ɍ��킹�ĉ�������
	if (jumpSpd > -FALL_VELOCITY_MAX)
	{
		jumpSpd -= STANDARD_GRAVITY;
	}

	// �W�F�b�g�p�b�N������or�p���[�A�b�v�o�i�i�g�p���̓W�����v�̓A�b�v
	if (jet || PowerBanana)
	{
		jumpValue = JETPACK_VALUE;
	}
	else
	{
		jumpValue = 1.0f;
	}
}

//=====================================================================================================
// �A�j���[�V�����̃Z�b�g
//=====================================================================================================
void Player::CreateAnimSet()
{
	ANIMATIONSET *AnimationSet = new ANIMATIONSET();
	vector<KEYDATA>Keydata;
	Keydata.reserve(Keydata_Max);

	for (int Set_No = 0; Set_No < GetAnimSetNum(); Set_No++)
	{
		switch (Set_No)
		{
		case Idle:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Idle", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Running:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Running", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Jump:

			Keydata.push_back(KEYDATA{ 0.80f, e_MotionEnd });
			AnimationSet->SetData("Jump", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Victory:

			AnimationSet->SetData("Victory", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Slip:

			Keydata.push_back(KEYDATA{ 0.95f, e_MotionChange });
			AnimationSet->SetData("Slip", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Stop:

			AnimationSet->SetData("Stop", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Lose:

			AnimationSet->SetData("Lose", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Clapping:

			AnimationSet->SetData("Clapping", NULL, 1.5f, 0.1f, 0.0f);
			break;

		default:
			break;
		}

		this->SetupCallbackKeys(&Keydata, AnimationSet->GetSetName());

		AnimationSet->SetAnimSetPtr(this->AnimController);

		this->AnimSet.push_back(*AnimationSet);

		Keydata.clear();
	}

	SAFE_DELETE(AnimationSet);
	ReleaseVector(Keydata);
}

//=====================================================================================================
// �A�j���[�V����CallbackKey�̏����C�x���g
//=====================================================================================================
HRESULT CALLBACK Player::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	int eventNo = (int)pCallbackData;

	switch (eventNo)
	{
	case e_MotionEnd:
		animSpd = 0.0f;
		break;
	case e_MotionChange:
		playable = true;
		ChangeAnim(Idle);
		ChangeState(new IdleState(this));
		break;
	default:
		break;
	}

	return S_OK;
}

//=====================================================================================================
// �J����������
//=====================================================================================================
void Player::CheckOnCamera()
{
	CAMERA *camera = GetCamera();

	// �c
	if (pos.x > camera->at.x - DRAW_RANGE.x)
	{
		onCamera = true;
	}
	else
	{
		onCamera = false;
		playerUI->SetPlayerDeadTexture();

		// �G�t�F�N�g����
		D3DXVECTOR3 setpos = pos;
		setpos.z -= 1.0f;
		setpos.x += 150.0f;
		std::vector<Effect3D*> *Effect3DVector = GetEffect3DVector();
		Effect3D *effect = new Effect3D(DeadEffect3D, setpos, 1);
		Effect3DVector->push_back(effect);

		PlaySound(SE_EXP);
	}
}

//=====================================================================================================
// �}�b�v�Ƃ̓����蔻��
//=====================================================================================================
void Player::GroundCollider()
{
	// �㏸���͔��肵�Ȃ�
	if (jumpSpd <= 0)
	{
		// �L�����N�^�[�̍��W����}�b�v�z��̏ꏊ�𒲂ׂ�
		int x, y;
		Map::GetMapChipXY(pos, &x, &y);

		D3DXVECTOR3 mappos = Map::GetMapChipPos(x, y, eCenterUp);

		// ���ݍ��W������Ƃ���ɂȂɂ��I�u�W�F�N�g������΃q�b�g���Ă���
		if (Map::GetMapTbl(x, y) >= 0)
		{
			// �߂荞�݂��C��
			pos.y = max(mappos.y - 0.01f, pos.y);
			jumpSpd = 0.0f;
			animSpd = 1.0f;
			hitGround = true;
			return;
		}
		else
		{
			hitGround = false;
		}
	}
	else
	{
		hitGround = false;
	}
}

//=====================================================================================================
// �y�C���g�Ƃ̓����蔻��
//=====================================================================================================
void Player::PaintCollider()
{
	// �㏸���͔��肵�Ȃ�
	if (jumpSpd <= 0)
	{
		for (auto &Paint : PaintSystem->GetColorPaint())
		{
			if (!Paint->GetUse())
				continue;

			// �ЂƂЂƂ̃y�C���g�ƃv���C���[�̓����蔻����s��
			if (HitSphere(pos, Paint->GetPos(), PLAYER_COLLISION_SIZE.x * 0.5f, PAINT_WIDTH * 0.5f))
			{
				// ���������ꍇ�A�v���C���[�̍��W���C��
				pos.y = max(Paint->GetPos().y + PAINT_WIDTH * 0.1f, pos.y);
				jumpSpd = 0.0f;
				animSpd = 1.0f;
				hitPaint = true;
				return;
			}
			else
			{
				hitPaint = false;
			}
		}
	}
	else
	{
		hitPaint = false;
	}
}

//=====================================================================================================
// �O���̃}�b�v�Ƃ̓����蔻��
//=====================================================================================================
void Player::HorizonCollider()
{
	// �L�����N�^�[�̍��W����}�b�v�z��̏ꏊ�𒲂ׂ�
	int x, y;
	Map::GetMapChipXY(pos, &x, &y);

	// �������猩�ĉE��Ȃ̂�
	x++;
	y--;

	// �e�[�u���𒲂ׂ�0�ȏ�Ȃ�q�b�g
	if (Map::GetMapTbl(x, y) >= 0)
	{
		hitHorizon = true;
		return;
	}
	else
	{
		hitHorizon = false;
		return;
	}
}

//=====================================================================================================
// �t�B�[���h�I�u�W�F�N�g�i�����j�Ƃ̓����蔻��
//=====================================================================================================
void Player::ObjectCollider()
{
	// �L�����N�^�[�̍��W����}�b�v�z��̏ꏊ�𒲂ׂ�
	int x, y;
	Map::GetMapChipXY(pos, &x, &y);

	int objType = Map::GetObjTbl(x, y);

	HitObjectInfluence(objType);
}

//=====================================================================================================
// �t�B�[���h�I�u�W�F�N�g�i�A�C�e���j�Ƃ̓����蔻��
//=====================================================================================================
void Player::ObjectItemCollider(Map *pMap)
{
	// �A�C�e�����擾���Ă����ԂȂ画�肵�Ȃ�
	if (hitItem)
	{
		return;
	}
	D3DXVECTOR3 colliderpos = pos;
	colliderpos.y += OBJECT_HIT_SIZE.y * 0.5f;

	for (auto &Obj : pMap->GetObjectChip())
	{
		if (Obj->GetTextureNum() != eObjItem)
		{
			continue;
		}

		if (HitCheckBB(colliderpos, Obj->GetPos(), OBJECT_HIT_SIZE, D3DXVECTOR2(CHIP_SIZE, CHIP_SIZE)))
		{
			ItemGetEffect(Obj->GetPos());
			hitItem = true;
			PlaySound(SE_PICITEM);
			return;
		}
	}

	hitItem = false;
	return;
}

//=====================================================================================================
// �t�B�[���h��ɔ��������A�C�e���Ƃ̓����蔻��
//=====================================================================================================
void Player::FieldItemCollider(FieldItemManager *pFIManager)
{
	for (auto &item : pFIManager->GetItem())
	{
		if (HitCheckBB(pos, item->GetPos(), PLAYER_COLLISION_SIZE, FIELDITEM_SIZE))
		{
			switch (item->GetTexNo())
			{
				// �o�i�i�̔�
			case NumKawa:
				playable = false;
				ChangeAnim(Slip);
				ChangeState(new SlipState(this));
				break;
				// �g�����`�K��
			case NumGun:
				playable = false;
				ChangeAnim(Stop);
				ChangeState(new StopState(this));
				break;
			default:
				break;
			}
			item->SetUse(false);

			// �G�t�F�N�g�𔭐�
			std::vector<Effect3D*> *Effect3DVector = GetEffect3DVector();
			Effect3D *effect = new Effect3D(ExplosionEffect3D, pos, 1);
			Effect3DVector->push_back(effect);

			PlaySound(SE_EXP);
		}
	}
}

//=====================================================================================================
// �I�u�W�F�N�g�Ƀq�b�g���Ă���Ƃ��̌���
//=====================================================================================================
void Player::HitObjectInfluence(int type)
{
	// �������݂��Ȃ��Ƃ�
	if (type == -1)
	{
		runSpd = 1.0f;
		jumpValue = 1.0f;
		hitObjCnt = 0;
		return;
	}

	// �I�u�W�F�N�g�̎�ނɍ��킹�Č��ʕύX
	switch (type)
	{
	case eObjSpdup:

		if (!spike)
		{
			runSpd = 2.0f;
		}

		// ���̃X�e�[�^�X�̓��Z�b�g
		hitObjCnt = 0;
		jumpValue = 1.0f;
		break;

	case eObjSpddown:

		if (!spike)
		{
			runSpd = 0.5f;
		}

		// ���̃X�e�[�^�X�̓��Z�b�g
		hitObjCnt = 0;
		jumpValue = 1.0f;
		break;

	case eObjNuma:

		if (!spike)
		{
			runSpd = 0.5f;
			jumpValue = 0.5f;
		}

		// ���̃X�e�[�^�X�̓��Z�b�g
		hitObjCnt = 0;
		break;

	case eObjJump:

		jumpSpd = JUMP_SPEED * jumpValue;
		ChangeAnim(Jump);
		ChangeState(new JumpState(this));

	case eObjDrain:

		// ���Ԍo�߂ŃC���N����
		if (!spike)
		{
			hitObjCnt = LoopCountUp(hitObjCnt, 0, OBJECT_HIT_COUNTER);
			if (hitObjCnt == 0)
			{
				int ink = PaintSystem->GetInkValue(BlackInk);
				PaintSystem->SetInkValue(max(--ink, 0), BlackInk);
				ink = PaintSystem->GetInkValue(ColorInk);
				PaintSystem->SetInkValue(max(--ink, 0), ColorInk);

				//PlaySound(�C���N�����鉹)
			}
		}

		// ���̃X�e�[�^�X�̓��Z�b�g
		runSpd = 1.0f;
		jumpValue = 1.0f;
		break;

	case eObjHeal:

		// ���Ԍo�߂ŃC���N����
		if (!spike)
		{
			hitObjCnt = LoopCountUp(hitObjCnt, 0, OBJECT_HIT_COUNTER);
			if (hitObjCnt == 0)
			{
				int ink = PaintSystem->GetInkValue(BlackInk);
				PaintSystem->SetInkValue(min(++ink, INK_MAX), BlackInk);
				ink = PaintSystem->GetInkValue(ColorInk);
				PaintSystem->SetInkValue(min(++ink, INK_MAX), ColorInk);

				// PlaySound(�C���N���񕜂��鉹)
			}
		}

		// ���̃X�e�[�^�X�̓��Z�b�g
		runSpd = 1.0f;
		jumpValue = 1.0f;
		break;

	case eObjItem:

		// ���̃X�e�[�^�X�̓��Z�b�g
		hitObjCnt = 0;
		runSpd = 1.0f;
		jumpValue = 1.0f;
		break;

	default:
		break;
	}
}

//=====================================================================================================
// �p���[�A�b�v�G�t�F�N�g�̕t�^
//=====================================================================================================
void Player::PowwrUpEffect()
{
	if (GetAnimCurtID() == Running || GetAnimCurtID() == Idle || GetAnimCurtID() == Jump)
	{
		D3DXVECTOR3 NewPos = pos;
		std::vector<Particle*> *vector = ParticleManager::GetParticle();

		NewPos.x += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.y += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.z += (float)(rand() % 20 - 10) * 0.1f;

		vector->push_back(new Particle(NewPos, D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXCOLOR(1.00f, 0.00f, 0.00f, 0.5f), D3DXVECTOR3(5.0f, 5.0f, 5.0f), 10));
		vector->push_back(new Particle(NewPos, D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXCOLOR(1.00f, 0.20f, 0.00f, 0.3f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), 15));
		vector->push_back(new Particle(NewPos, D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXCOLOR(1.00f, 0.40f, 0.00f, 0.15f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 20));
	}
}

//=====================================================================================================
// �����j���O�G�t�F�N�g�̕t�^
//=====================================================================================================
void Player::RunningEffect()
{
	// �󒆂Ȃ甭�����Ȃ�
	if (!hitGround && !hitPaint)
		return;

	// ��������A�C�e�����g���Ă���Œ��Ȃ�G�t�F�N�g�𔭐������Ȃ�
	if (blind || spike || PowerBanana || jet || PaintSystem->GetSpInk())
		return;

	std::vector<Particle*> *vector = ParticleManager::GetParticle();
	vector->push_back(new Particle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.00f, 0.85f, 0.73f, 0.15f), D3DXVECTOR3(5.0f, 5.0f, 5.0f), 20));
	vector->push_back(new Particle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.00f, 0.85f, 0.73f, 0.15f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), 40));
	vector->push_back(new Particle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.00f, 0.85f, 0.73f, 0.15f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 60));
}

//=====================================================================================================
// �A�C�e���擾�G�t�F�N�g�̕t�^
//=====================================================================================================
void Player::ItemGetEffect(D3DXVECTOR3 pos)
{
	std::vector<Particle*> *particle = ParticleManager::GetParticle();

	D3DXVECTOR3 move;

	for (int i = 0; i < 10; i++)
	{
		move.x = RandomRange(-3.0f, 3.0f);
		move.y = RandomRange(-3.0f, 3.0f);
		move.z = 0.0f;
		particle->push_back(new Particle(pos, move, MyColor[nYellow], D3DXVECTOR3(3.0f, 3.0f, 3.0f), 30));
	}

}

//=====================================================================================================
// �f�o�b�O�\��&����
//=====================================================================================================
void Player::Debug()
{
#if _DEBUG
	if (!AIUse)
	{
		if (GetKeyboardPress(DIK_RIGHT))
		{
			switch (ctrlNum)
			{
			case 0:
				pos.x += MOVE_SPEED;
				break;
			case 1:
				pos.x += MOVE_SPEED * 0.8f;
				break;
			case 2:
				pos.x += MOVE_SPEED * 0.5f;
				break;
			case 3:
				pos.x += MOVE_SPEED * 0.2f;
				break;
			default:
				break;
			}
		}
		if (GetKeyboardPress(DIK_LEFT))
		{
			switch (ctrlNum)
			{
			case 0:
				pos.x -= MOVE_SPEED;
				break;
			case 1:
				pos.x -= MOVE_SPEED * 0.8f;
				break;
			case 2:
				pos.x -= MOVE_SPEED * 0.5f;
				break;
			case 3:
				pos.x -= MOVE_SPEED * 0.2f;
				break;
			default:
				break;
			}
		}
	}

	ImGui::SetNextWindowPos(ImVec2(5, 120), ImGuiSetCond_Once);

	BeginDebugWindow("Player");

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode((void*)(intptr_t)ctrlNum, "Player %d", ctrlNum))
	{
		if (ImGui::TreeNode("Position"))
		{
			DebugText("Pos X:%.2f\nPos Y:%.2f\nPos Z:%.2f\n", pos.x, pos.y, pos.z);
			ImGui::TreePop();
		}

		DebugText("AnimSetName:%s\nCurrentFrame:%d / %d", this->GetCurtAnimName(), this->GetAnimCurtFrame(), this->GetAnimPeriodFrame());

		int x = 0, y = 0;
		Map::GetMapChipXY(pos, &x, &y);
		DebugText("X : %d  Y : %d", x, y);
		DebugText("MapTable : %d\nMapTable_Up : %d", Map::GetMapTbl(pos, eCenter), Map::GetMapTbl(pos, eCenterUp));

		ImGui::TreePop();
	}

	EndDebugWindow("Player");
#endif

}