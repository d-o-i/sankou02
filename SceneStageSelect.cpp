//=============================================================================
//
// �X�e�[�W�Z���N�g [SceneStageSelect.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneStageSelect.h"
#include "SceneManager.h"
#include "SceneCharacterSelect.h"
#include "Input.h"
#include "CircleSceneChanger.h"

#include "StageSelectBG.h"
#include "StageName.h"
#include "StageSelectCursor.h"
#include "StageSelectLogo.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneStageSelect::SceneStageSelect()
{
	// �w�i
	obj.push_back(new StageSelectBG());
	// �J�[�\��
	obj.push_back(new StageSelectCursor());
	// �X�e�[�W��
	obj.push_back(new StageName());
	// ���S
	obj.push_back(new StageSelectLogo());

/*****************************************************************************/
	// �V�[���`�F���W�̏I��
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneStageSelect::~SceneStageSelect()
{
	// 2D�I�u�W�F�N�g�̍폜
	for (auto &UI : obj)
	{
		SAFE_DELETE(UI);
	}
	obj.clear();
	ReleaseVector(obj);

}

//=============================================================================
// �X�V
//=============================================================================
void SceneStageSelect::Update(int SceneID)
{
	// �V�[���J��
	for (int playerNo = 0; playerNo < GAMEPAD_MAX; playerNo++)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(playerNo, BUTTON_C))
		{
			CircleSceneChanger::Instance()->SetChanger(true, []() 
			{
				SetScene(nSceneCharacterSelect);
			});
			return;
		}
	}

	// 2D�I�u�W�F�N�g�̍X�V
	for (auto & UI : obj)
	{
		UI->Update();
	}
}

//=============================================================================
// �`��
//=============================================================================
void SceneStageSelect::Draw()
{
	// 2D�I�u�W�F�N�g�̕`��
	for (auto & UI : obj)
	{
		UI->Draw();
	}
}