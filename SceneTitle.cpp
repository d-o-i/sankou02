//=============================================================================
//
// �^�C�g����ʏ��� [SceneTitle.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneTitle.h"
#include "_2dobj.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "Sound.h"
#include "SceneCharacterSelect.h"
#include "TitleCursor.h"
#include "SceneTutorial.h"
#include "Player.h"
#include "CircleSceneChanger.h"
#include "SceneExit.h"
#include "Sky.h"
#include "Camera.h"
#include "Runner.h"
#include "ParticleManager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneTitle::SceneTitle()
{
	
	IsOption = true;							// �I�����t���O�I��

	// UI�I�u�W�F�N�g
	p2dObj.push_back(new TITLE(TitleLogo));
	p2dObj.push_back(new TITLE(TitleMenu));
	p2dObj.push_back(new Runner());
	p2dObj.push_back(new TITLECURSOR());

	// �}�b�v
	map = new Map();

	// 3D�I�u�W�F�N�g
	object3d.push_back(new Sky());

	// �p�[�e�B�N���}�l�[�W��
	particleManager = new ParticleManager();

/*****************************************************************************/
	// �V�[���`�F���W�̏I��
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneTitle::~SceneTitle()
{
	// 2D�I�u�W�F�N�g�̊J��
	for (auto &UI : p2dObj)
	{
		SAFE_DELETE(UI);
	}
	p2dObj.clear();
	ReleaseVector(p2dObj);

	// �}�b�v�̊J��
	SAFE_DELETE(map);

	// 3D�I�u�W�F�N�g�̊J��
	for (auto &obj : object3d)
	{
		SAFE_DELETE(obj);
	}
	object3d.clear();
	ReleaseVector(object3d);

	// �p�[�e�B�N���}�l�[�W���̊J��
	SAFE_DELETE(particleManager);
}

//=============================================================================
// �X�V
//=============================================================================
void SceneTitle::Update(int SceneID)
{
	// ���[�h�I��
	for (int playerNo = 0; playerNo < PLAYER_MAX; playerNo++)
	{
		if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(playerNo, STICK_UP))
		{
			IsOption = true;
			break;
		}
		else if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(playerNo, STICK_DOWN))
		{
			IsOption = false;
			break;
		}

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(playerNo, BUTTON_C))
		{
			if (IsOption == true)
			{
				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene(nSceneTutorial);
					InitCamera();
				});
				return;
			}
			else
			{
				//SetScene(new SceneExit(), nSceneExit);

				CircleSceneChanger::Instance()->SetChanger(true, []()
				{
					SetScene( nSceneExit);
				});
				return;
			}
		}


	}

	// �J�����̍X�V
	UpdateTitleCamera();

	// 2D�I�u�W�F�N�g�̍X�V
	for (auto &UI : p2dObj)
	{
		UI->Update();
	}

	// �}�b�v�̍X�V
	map->Update();

	// 3D�I�u�W�F�N�g�̍X�V
	for (auto &obj : object3d)
	{
		obj->Update();
	}

	// �p�[�e�B�N���}�l�[�W���̍X�V
	particleManager->Update();
}

//=============================================================================
// �`��
//=============================================================================
void SceneTitle::Draw()
{
	// �}�b�v�̕`��
	map->Draw();

	// 3D�I�u�W�F�N�g�̕`��
	for (auto &obj : object3d)
	{
		obj->Draw();
	}

	// 2D�I�u�W�F�N�g�̕`��
	for (auto &UI : p2dObj)
	{
		UI->Draw();
	}

	// �p�[�e�B�N���}�l�[�W���̕`��
	particleManager->Draw();
}