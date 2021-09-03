//=============================================================================
//
// �`���[�g���A���V�[�� [SceneTutorial.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneTutorial.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "Input.h"
#include "CircleSceneChanger.h"
#include "TutorialLogo.h"
#include "Arrow.h"
#include "Tutorial.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneTutorial::SceneTutorial()
{
	// ����
	obj.push_back(new Tutorial());

	// ���S
	obj.push_back(new TutorialLogo());

	// ���E�I����
	obj.push_back(new Arrow(true));
	obj.push_back(new Arrow(false));


/*****************************************************************************/
	// �V�[���`�F���W�̏I��
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneTutorial::~SceneTutorial()
{
	// 2D�I�u�W�F�N�g�̊J��
	for (auto &Object : obj)
	{
		SAFE_DELETE(Object);
	}
	obj.clear();
	ReleaseVector(obj);
}

//=============================================================================
// �X�V
//=============================================================================
void SceneTutorial::Update(int SceneID)
{
	// �V�[���؂�ւ�
	for (int padNo = 0; padNo < GAMEPAD_MAX; padNo++)
	{
		if (IsButtonTriggered(padNo, BUTTON_C) || GetKeyboardTrigger(DIK_RETURN))
		{
			CircleSceneChanger::Instance()->SetChanger(true, []() 
			{
				SetScene(nSceneStageSelect);
			});
			break;
		}
	}

	// 2D�I�u�W�F�N�g�̍X�V
	for (auto Object : obj)
	{
		Object->Update();
	}
}

//=============================================================================
// �`��
//=============================================================================
void SceneTutorial::Draw()
{
	// 2D�I�u�W�F�N�g�̕`��
	for (auto Object : obj)
	{
		Object->Draw();
	}
}