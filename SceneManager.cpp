//=============================================================================
//
// �V�[���}�l�[�W�� [SceneManager.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneManager.h"
#include "Input.h"
#include "Camera.h"
#include "Light.h"
#include "Sound.h"
#include "SceneTitle.h"
#include "SceneCharacterSelect.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneExit.h"
#include "SceneStageSelect.h"
#include "SceneTutorial.h"
#include "ResourceManager.h"
#include "CircleSceneChanger.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int eScene = nSceneTitle;				// �Q�[���̊J�n�ʒu&�V�[���J��
static Scene *scene;						// �Q�[���V�[���̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneManager::SceneManager(HINSTANCE hInstance, HWND hWnd)
{
	InitInput(hInstance, hWnd);
	InitCamera();
	InitLight();
	InitSound(hWnd);

	// ���\�[�X�̓ǂݍ���
	LoadResource();

	// �J�n�ʒu�ɍ��킹�ēǂݍ���
	switch (eScene)
	{
	case nSceneTitle:
		PlaySound(BGM_TITLE);
		scene = new SceneTitle();
		break;
	case nSceneTutorial:
		scene = new SceneTutorial();
		break;
	case nSceneStageSelect:
		scene = new SceneStageSelect();
		break;
	case nSceneCharacterSelect:
		PlaySound(BGM_CHARSEL);
		scene = new SceneCharacterSelect();
		break;
	case nSceneGame:
		PlaySound(BGM_TRAINING);
		scene = new SceneGame();
		break;
	case nSceneResult:
		scene = new SceneResult();
		break;
	case nSceneExit:
		scene = new SceneExit();
	default:
		break;
	}

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneManager::~SceneManager()
{
	delete scene;
	UninitSound();

	// ���\�[�X�̍폜
	ResourceManager::Instance()->AllRelease();

	UninitInput();
}

//=============================================================================
// �X�V
//=============================================================================
void SceneManager::Update()
{
	if (!CircleSceneChanger::Instance()->GetUseMask())
	{
		UpdateInput();
	}

	scene->Update(eScene);

	CircleSceneChanger::Instance()->Update();
}

//=============================================================================
// �`��
//=============================================================================
void SceneManager::Draw()
{
	SetCamera();

	// �X�e���V���}�X�N�̕`��
	CircleSceneChanger::Instance()->DrawMask();

	scene->Draw();

	// �V�[���`�F���W���[�̕`��
	CircleSceneChanger::Instance()->DrawChanger();

}

//=============================================================================
// ���\�[�X�̓ǂݍ���
//=============================================================================
void SceneManager::LoadResource()
{
	// ���\�[�X�쐬
	// ����1:���ʂ̂��߂̃^�O��(�Ăяo����ƍ��킹��) ����2:�t�@�C���̃p�X
	// SceneChanger
	ResourceManager::Instance()->LoadTexture("Mask", "data/TEXTURE/Circle.png");
	ResourceManager::Instance()->LoadTexture("Changer", "data/TEXTURE/Load.png");
	CircleSceneChanger::Instance()->LoadMaskTexture();
	CircleSceneChanger::Instance()->LoadChangeTexture();

	// SceneTitle
	ResourceManager::Instance()->LoadTexture("TitleLogo", "data/TEXTURE/Logo.png");
	ResourceManager::Instance()->LoadTexture("TitleRunner", "data/TEXTURE/Runner.png");
	ResourceManager::Instance()->LoadTexture("TitleMenu", "data/TEXTURE/TitleLogo.png");
	ResourceManager::Instance()->LoadTexture("TitleCursor", "data/TEXTURE/TitleCursor.png");

	// SceneTutorial
	ResourceManager::Instance()->LoadTexture("TutorialLogo", "data/TEXTURE/TutorialLogo.png");
	ResourceManager::Instance()->LoadTexture("Tutorial", "data/TEXTURE/Tutorial.png");
	ResourceManager::Instance()->LoadTexture("Arrow", "data/TEXTURE/Arrow.png");

	// SceneStageSelect
	ResourceManager::Instance()->LoadTexture("StageSelectBG", "data/TEXTURE/StageSelectBG.png");
	ResourceManager::Instance()->LoadTexture("StageName", "data/TEXTURE/StageName.png");
	ResourceManager::Instance()->LoadTexture("StageSelectLogo", "data/TEXTURE/StageSelectLogo.png");

	// SceneCharacterSelect
	ResourceManager::Instance()->LoadTexture("SelectLogo", "data/TEXTURE/CharSelectLogo.png");
	ResourceManager::Instance()->LoadTexture("SelectCursor", "data/TEXTURE/CharSelectCursor.png");	// SceneGame��Face�ɂ��g��

	// SceneGame
	ResourceManager::Instance()->LoadTexture("UIFrame", "data/texture/frame000.png");
	ResourceManager::Instance()->LoadTexture("CountDown", "data/TEXTURE/countdown.png");
	ResourceManager::Instance()->LoadTexture("Item", "data/TEXTURE/Item.png");
	ResourceManager::Instance()->LoadTexture("Finish", "data/TEXTURE/Finish.png");
	ResourceManager::Instance()->LoadTexture("Sky", "data/MAP/BG000.png");
	ResourceManager::Instance()->LoadTexture("GoalFlag", "data/TEXTURE/GoalFlag.png");
	ResourceManager::Instance()->LoadTexture("Digit", "data/TEXTURE/Digit.png");					// SceneResult��ResultDigit�ɂ��g��
	ResourceManager::Instance()->LoadTexture("MapChip", "data/MAP/tilea5.png");
	ResourceManager::Instance()->LoadTexture("ObjectChip", "data/MAP/mapchip_object.png");
	ResourceManager::Instance()->LoadTexture("HitEffect", "data/EFFECT/anmef000.png");
	ResourceManager::Instance()->LoadTexture("DeadEffect", "data/EFFECT/anmef001.png");
	ResourceManager::Instance()->LoadTexture("RunEffect", "data/EFFECT/anmef002.png");
	ResourceManager::Instance()->LoadTexture("ExplosionEffect", "data/EFFECT/explo000.png");
	ResourceManager::Instance()->LoadTexture("ItemEffect", "data/EFFECT/ief001.png");
	ResourceManager::Instance()->LoadTexture("Item1Ettect", "data/EFFECT/ief000.png");
	ResourceManager::Instance()->LoadTexture("ChargeEffect", "data/EFFECT/Charge.png");
	ResourceManager::Instance()->LoadTexture("Paint", "data/TEXTURE/paint.png");
	ResourceManager::Instance()->LoadTexture("Cursor", "data/TEXTURE/brush.png");
	ResourceManager::Instance()->LoadTexture("InkGauge", "data/texture/InkGauge.png");
	ResourceManager::Instance()->LoadTexture("ColorInkFrame", "data/texture/colorinkframe.png");
	ResourceManager::Instance()->LoadTexture("BlackInkFrame", "data/texture/blackinkframe.png");
	ResourceManager::Instance()->LoadTexture("FaceFrame", "data/texture/faceframe.png");
	ResourceManager::Instance()->LoadTexture("MiniPlayer", "data/TEXTURE/MiniPlayer.png");
	ResourceManager::Instance()->LoadTexture("Pop", "data/TEXTURE/pointer.png");
	ResourceManager::Instance()->LoadTexture("FieldItem", "data/TEXTURE/FieldItem.png");

	// SceneResult
	ResourceManager::Instance()->LoadTexture("ResultRank", "data/TEXTURE/ResultPlayer.png");
	ResourceManager::Instance()->LoadTexture("SkyBox", "data/TEXTURE/SkyBox.png");
	ResourceManager::Instance()->LoadTexture("MeshField", "data/TEXTURE/Block.jpg");
	ResourceManager::Instance()->LoadTexture("Result", "data/TEXTURE/Result.png");

}

//=====================================================================================================
// �V�[���J��
//=====================================================================================================
void SetScene(int _scene)
{
	eScene = _scene;
	SAFE_DELETE(scene)

	switch (_scene)
	{
	case nSceneTitle:
		scene = new SceneTitle();
		break;
	case nSceneTutorial:
		scene = new SceneTutorial();
		break;
	case nSceneStageSelect:
		scene = new SceneStageSelect();
		break;
	case nSceneCharacterSelect:
		scene = new SceneCharacterSelect();
		break;
	case nSceneGame:
		scene = new SceneGame();
		break;
	case nSceneResult:
		scene = new SceneResult();
		break;
	case nSceneExit:
		scene = new SceneExit();
		break;
	default:
		break;
	}
//<<<<<<< HEAD
//	delete scene;
//	scene = NewScene;
//=======
//>>>>>>> Develop
}

//=====================================================================================================
// �V�[���̃Q�b�^�[
//=====================================================================================================
int GetScene()
{
	return eScene;
}
