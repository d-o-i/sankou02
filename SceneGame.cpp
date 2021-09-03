//=============================================================================
//
// �Q�[����ʏ��� [SceneGame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Map.h"
#include "Camera.h"
#include "Collision.h"
#include "Input.h"
#include "DebugWindow.h"
#include "SceneResult.h"
#include "CircleSceneChanger.h"

// 2d obj
#include "Frame01.h"
#include "Face.h"
#include "CountDown.h"
#include "Item.h"
#include "Finish.h"

// 3d obj
#include "Sky.h"
#include "GoalFlag.h"

#include "Sound.h"
static int ResultRank[PLAYER_MAX];
//*****************************************************************************
// �����o�ϐ��̏�����
//*****************************************************************************
ResultData SceneGame::data[PLAYER_MAX] = { NULL };		// ����
int SceneGame::TheLastPlayer = 0;		// ����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneGame::SceneGame()
{
	// �Q�[���̌��ʂ�������
	startframe = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		data[i].playerNo = -1;
		data[i].time = 0;
	}
	result = false;

	// �}�b�v�̏�����
	pMap = new Map();

	// �l���؂̏�����
	Quadtree = new QUADTREE(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	PaintManager::SetQuadtreePtr(Quadtree);

	// �y�C���g�O���[�v�̏�����
	paintGroup = new PaintGroup();
	PaintManager::SetPaintGroupPtr(paintGroup);

#if _DEBUG
	pPlayer[0] = new Player(0, true);
	pPlayer[1] = new Player(1, true);
	//pPlayer[1]->SetOnCamera(false);
	pPlayer[2] = new Player(2, true);
	//pPlayer[2]->SetOnCamera(false);
	pPlayer[3] = new Player(3, true);
	//pPlayer[3]->SetOnCamera(false);
#else
	// �v���C���[�̏�����
	for (int PlayerNo = 0; PlayerNo < PLAYER_MAX; PlayerNo++)
	{
		pPlayer[PlayerNo] = new Player(PlayerNo, false);
	}
#endif

	// 2DUI�̏�����
	// �t���[��
	UIObject.push_back(new Frame());

	// �A�C�e���\���̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		UIObject.push_back(new Item(pPlayer[i]));
	}

	// �J�E���g�_�E���̏�����
	UIObject.push_back(new CountDown());

	// �G�t�F�N�g�}�l�[�W��
	pEffectManager = new EffectManager();

	// 3D�I�u�W�F�N�g
	object3d.push_back(new Sky());
	object3d.push_back(new GoalFlag());

	// �^�C�}�[
	pTimer = new Timer();

	// �p�[�e�B�N���}�l�[�W��
	particleManager = new ParticleManager();

	/*****************************************************************************/
		// �V�[���`�F���W�̏I��
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneGame::~SceneGame()
{
	// �}�b�v�̍폜
	SAFE_DELETE(pMap);

	// �l���؂̍폜
	SAFE_DELETE(Quadtree);
	PaintManager::ReleaseQuadtreePtr();

	// �y�C���g�O���[�v�̍폜
	SAFE_DELETE(paintGroup);
	PaintManager::ReleasePaintGroupPtr();

	// �v���C���[�̍폜
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		SAFE_DELETE(pPlayer[i]);
	}

	// 2D�I�u�W�F�N�g�̍폜
	for (auto &Object : UIObject)
	{
		SAFE_DELETE(Object);
	}
	UIObject.clear();
	ReleaseVector(UIObject);

	// �G�t�F�N�g�}�l�[�W���̍폜
	SAFE_DELETE(pEffectManager);

	// 3D�I�u�W�F�N�g�̍폜
	for (auto &Obj3D : object3d)
	{
		SAFE_DELETE(Obj3D);
	}
	object3d.clear();
	ReleaseVector(object3d);

	// �^�C�}�[�̍폜
	SAFE_DELETE(pTimer);

	// �p�[�e�B�N���}�l�[�W���̍폜
	SAFE_DELETE(particleManager);
}

//=============================================================================
// �X�V
//=============================================================================
void SceneGame::Update(int SceneID)
{
	// �J�n����
	if (startframe < START_FRAME)
	{
		Start();
	}

	float MaxPosX = 0.0f;
	float MinPosX = 100000.0f;
	int FirstPlayer = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			// �v���C���[���W�̒���X���ł��傫�����̂��J���������_�Ƃ���
			if (pPlayer[i]->GetPos().x > MaxPosX)
			{
				MaxPosX = pPlayer[i]->GetPos().x;
				FirstPlayer = i;
			}

			// �v���C���[���W�̒���X���ł�������
			if (pPlayer[i]->GetPos().x < MinPosX)
			{
				MinPosX = pPlayer[i]->GetPos().x;
				SceneGame::TheLastPlayer = i;
			}

			// �O���Ƀv���C���[�����邩�ǂ������m�F
			if (pPlayer[i]->GetAIUse())
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// ���݂̃v���C���[�Ƃ͈Ⴄ
					if (i != j && pPlayer[j]->GetOnCamera())
					{
						// ���݂̃v���C���[���E�A���������������炢
						if (pPlayer[i]->GetPos().x < pPlayer[j]->GetPos().x &&
							fabsf(pPlayer[i]->GetPos().y - pPlayer[j]->GetPos().y) < 10.0f)
						{
							pPlayer[i]->GetAIPtr()->SetShotBullet(true);
							break;
						}
						else
						{
							pPlayer[i]->GetAIPtr()->SetShotBullet(false);
						}
					}
				}
			}

		}
	}

	// �J�����̍X�V
	UpdateCamera(pPlayer[FirstPlayer]->GetPos());

#if 0
	std::vector<float> vec(PLAYER_MAX);
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			vec.at(i) = pPlayer[i]->GetModel()->pos.x;
		}
	}
	auto max = std::max_element(vec.begin(), vec.end());
	size_t maxIdx = std::distance(vec.begin(), max);

	auto min = std::min_element(vec.begin(), vec.end());
	SceneGame::TheLastPlayer = (int)std::distance(vec.begin(), min);

	// �J�����̍X�V
	UpdateCamera(pPlayer[(int)maxIdx]->GetModel()->pos);
#endif

	// �}�b�v�̍X�V
	pMap->Update();

	// �v���C���[�̍X�V
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		pPlayer[i]->Update();
	}

	// �����蔻��̍X�V
	Collision();

	// �y�C���g�O���[�v�̍X�V
	paintGroup->Update();

	// 2D�I�u�W�F�N�g�̍X�V
	for (auto &Object : UIObject)
	{
		Object->Update();
	}

	// �G�t�F�N�g�}�l�[�W���̍X�V
	pEffectManager->Update();

	// 3D�I�u�W�F�N�g�̍X�V
	for (auto &Obj3D : object3d)
	{
		Obj3D->Update();
	}

	// �^�C�}�[�̍X�V
	pTimer->Update();

	// �p�[�e�B�N���}�l�[�W���̍X�V
	particleManager->Update();

	// ���U���g��ʂ֑J�ڂ��Ă������m�F
	CheckResult();

	// �f�o�b�O
	Debug();
}

//=============================================================================
// �`��
//=============================================================================
void SceneGame::Draw()
{
	// �}�b�v�̕`��
	pMap->Draw();

	// 3D�I�u�W�F�N�g�̕`��
	for (auto &Obj3D : object3d)
	{
		Obj3D->Draw();
	}

	// �v���C���[�̕`��
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		pPlayer[i]->Draw();
	}

	// �G�t�F�N�g�}�l�[�W���̕`��
	pEffectManager->Draw();

	// 2D�I�u�W�F�N�g�̕`��
	for (auto &Object : UIObject)
	{
		Object->Draw();
	}

	// �p�[�e�B�N���}�l�[�W���̕`��
	particleManager->Draw();

	// �^�C�}�[�̕`��
	pTimer->Draw();
}

//=============================================================================
// �����蔻��̍X�V
//=============================================================================
void SceneGame::Collision()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (pPlayer[i]->GetOnCamera())
		{
			// �v���C���[�ƃ}�b�v�̓����蔻��
			pPlayer[i]->GroundCollider();
			pPlayer[i]->HorizonCollider();
			pPlayer[i]->ObjectCollider();
			pPlayer[i]->ObjectItemCollider(pMap);

			// �v���C���[�ƃy�C���g�}�l�[�W���̓����蔻��
			pPlayer[i]->PaintCollider();
		}
	}

	// �y�C���g�}�l�[�W�����m�̓����蔻��
	for (int TenDigit = 1; TenDigit <= 4; TenDigit++)
	{
		for (int OneDigit = 1; OneDigit <= 4; OneDigit++)
		{
			std::vector<Paint*> CollisionList = Quadtree->GetObjectsAt((TenDigit * 10 + OneDigit));

			// ���݂̃m�[�h�̓I�u�W�F�N�g���Ȃ�
			if (CollisionList.empty())
			{
				continue;
			}

			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (pPlayer[i]->GetOnCamera())
				{
					// ��ʂ�16�����A���ꂼ��̃I�u�W�F�N�g�𔻒肷��
					HitCheckSToS(&CollisionList, i);
				}
			}
		}
	}

	// �t�B�[���h��ɔ��������A�C�e���Ƃ̓����蔻��
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nItem = 0; nItem < PLAYER_MAX; nItem++)
		{
			if (nPlayer != nItem)
			{
				pPlayer[nPlayer]->FieldItemCollider(pPlayer[nItem]->GetFieldItemManager());
			}
		}
	}

	// �t�B�[���h�I�u�W�F�N�g�ƃy�C���g�}�l�[�W���̓����蔻��
	for (int TenDigit = 1; TenDigit <= 4; TenDigit++)
	{
		for (int OneDigit = 1; OneDigit <= 4; OneDigit++)
		{
			// ��ʂ�16�����A���ꂼ��̃I�u�W�F�N�g�𔻒肷��
			pMap->PaintCollider(Quadtree, (TenDigit * 10 + OneDigit));
		}
	}

	// �l���؂��X�V����
	Quadtree->Update();

}

//=============================================================================
// �J�n����
//=============================================================================
void SceneGame::Start()
{
	// �X�^�[�g�^�C�}�[�X�V
	startframe++;

	if (startframe == START_FRAME)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			pPlayer[i]->SetPlayable(true);
		}

		pTimer->Start();
	}
}

//=============================================================================
// ���U���g��ʂ֑J�ڂ��Ă������m�F
//=============================================================================
void SceneGame::CheckResult()
{
#if _DEBUG
	if (GetKeyboardTrigger(DIK_N))
	{
		CircleSceneChanger::Instance()->SetChanger(true, []()
		{
			SetScene(nSceneResult);
			InitCamera();
		});
		return;
	}
#endif

	// �S���S�[��or�Q�[���I�[�o�[�Ȃ�V�[���J�ډ\
	if (result)
	{
		// �^�C�}�[�X�g�b�v
		pTimer->Stop();

		for (int pNo = 0; pNo < PLAYER_MAX; pNo++)
		{
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(pNo, BUTTON_C))
			{
				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene(nSceneResult);
					InitCamera();
				});
				return;
			}
		}

		return;	// �S���S�[�����Ă����炱�������̏����͂��Ȃ�
	}

	// �S�����S�[��or�Q�[���I�[�o�[�ɂȂ������m�F
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (data[i].playerNo != -1)
		{
			result = true;
		}
		else
		{
			result = false;
			break;
		}
	}

	// �S���S�[�������u�Ԃ̂�
	if (result)
	{
		UIObject.push_back(new Finish());
	}

	for (int pNo = 0; pNo < PLAYER_MAX; pNo++)
	{
		bool hit = false;
		// ���łɂ��̃v���C���[�̌��ʂ����U���g���ʔz��ɓo�^����Ă��邩�m�F
		for (int rNo = 0; rNo < PLAYER_MAX; rNo++)
		{
			if (data[rNo].playerNo != pNo)
			{
				hit = false;
			}
			else
			{
				hit = true;
				break;
			}
		}

		if (!hit)
		{
			// �܂����ʂ��o�^����Ă��Ȃ��ꍇ
			InsertResult(pNo);
		}
	}
}

//=============================================================================
// ���U���g���ʔz��Ƀf�[�^�̑}��
//=============================================================================
void SceneGame::InsertResult(int pNo)
{
	// �Q�[���I�[�o�[�m�F
	if (!pPlayer[pNo]->GetOnCamera())
	{
		// ���U���g���ʔz��̌�납�����Ă���
		for (int rNo = PLAYER_MAX - 1; rNo > 0; rNo--)
		{
			if (data[rNo].playerNo == -1)
			{
				data[rNo].playerNo = pNo;
				data[rNo].time = 359999;
				break;
			}
		}
	}

	// �S�[���m�F
	if (pPlayer[pNo]->GetPos().x >= GOAL_POS.x)
	{
		// ���U���g���ʔz��̑O�������Ă���
		for (int rNo = 0; rNo < PLAYER_MAX; rNo++)
		{
			if (data[rNo].playerNo == -1)
			{
				data[rNo].playerNo = pNo;
				data[rNo].time = pTimer->Check();
				break;
			}
		}
	}
}

//=============================================================================
// �f�o�b�O
//=============================================================================
void SceneGame::Debug()
{
#if _DEBUG_
	BeginDebugWindow("Result");

	DebugText("All Goal or Gameover : %s", result ? "True" : "False");
	DebugText("No1:%d No2:%d No3:%d No4:%d", data[0].playerNo, data[1].playerNo, data[2].playerNo, data[3].playerNo);
	DebugText("ResultTime\nNo1:%d No2:%d No3:%d No4:%d", data[0].time, data[1].time, data[2].time, data[3].time);

	EndDebugWindow("Result");

#endif
}

//=============================================================================
// ���ʂ̃Q�b�^�[
//=============================================================================
ResultData *SceneGame::GetResultData(int rank)
{
	return &data[rank];
}
