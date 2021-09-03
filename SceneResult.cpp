//=============================================================================
//
// �Q�[����ʏ��� [SceneGame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneResult.h"
#include "Camera.h"
#include "Result.h"
#include "Input.h"
#include "SceneGame.h"
#include "Player.h"
#include "DebugWindow.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "ResultRank.h"
#include "ResultTimer.h"
#include "Podium.h"
#include "Trophy.h"
#include "ResultPlayer.h"
#include "SkyBox.h"
#include "MeshField.h"
#include "CircleSceneChanger.h"
#include "Confetti.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SceneResult::SceneResult()
{
	// 2D�I�u�W�F�N�g�̃C���X�^���X���쐬
	Obj2d.push_back(new RESULT());
	// 1�ʂ��珇�ɐ���
	for (int resultNo = 0; resultNo < PLAYER_MAX; resultNo++)
	{
		Obj2d.push_back(new ResultRank(resultNo, SceneGame::GetResultData(resultNo)->playerNo));
		Obj2d.push_back(new ResultTimer(SceneGame::GetResultData(resultNo)->time, resultNo));
	}

	// 3D���f���̃C���X�^���X�쐬
	anim.push_back(new Podium());
	anim.push_back(new Trophy());
	// 1�ʂ��珇�ɐ���
	for (int resultNo = 0; resultNo < PLAYER_MAX; resultNo++)
	{
		anim.push_back(new ResultPlayer(resultNo, SceneGame::GetResultData(resultNo)->playerNo));
	}

	// 3D�|���S���̃C���X�^���X�쐬
	Obj3d.push_back(new SkyBox());
	Obj3d.push_back(new MeshField());

	// �p�[�e�B�N���}�l�[�W���̃C���X�^���X�쐬
	particleManager = new ParticleManager();

/*****************************************************************************/
	// �V�[���`�F���W�̏I��
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SceneResult::~SceneResult()
{
	// 2D�I�u�W�F�N�g�̍폜
	for (auto &Obj : Obj2d)
	{
		SAFE_DELETE(Obj);
	}
	Obj2d.clear();
	ReleaseVector(Obj2d);

	// 3D���f���̍폜
	for (auto &Anim : anim)
	{
		SAFE_DELETE(Anim);
	}
	anim.clear();
	ReleaseVector(anim);

	// 3D�|���S���̍폜
	for (auto &Obj : Obj3d)
	{
		SAFE_DELETE(Obj);
	}
	Obj3d.clear();
	ReleaseVector(Obj3d);

	// �p�[�e�B�N���}�l�[�W���̍폜
	delete particleManager;
}

//=============================================================================
// �X�V
//=============================================================================
void SceneResult::Update(int SceneID)
{
	// �J�����̍X�V
	UpdateCamera();

	// �V�[���`�F���W
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(i, BUTTON_C))
		{
			CircleSceneChanger::Instance()->SetChanger(true, [] 
			{
				SetScene(nSceneTitle);
				InitCamera();
			});
			return;
		}
	}

	// 2D�I�u�W�F�N�g�̍X�V
	for (auto &Obj : Obj2d)
	{
		Obj->Update();
	}

	// 3D���f���̍X�V
	for (auto &Anim : anim)
	{
		Anim->Update();
	}

	// 3D�|���S���̍X�V
	for (auto &Obj : Obj3d)
	{
		Obj->Update();
	}

	// ���t���[���p�[�e�B�N���𔭐�������
	particleManager->SetConfetti();

	// �p�[�e�B�N���}�l�[�W���̍X�V
	particleManager->Update();

#if _DEBUG
	//Debug();
#endif
}

//=============================================================================
// �`��
//=============================================================================
void SceneResult::Draw()
{
	// 3D���f���̕`��
	for (auto &Anim : anim)
	{
		Anim->Draw();
	}

	// 3D�|���S���̕`��
	for (auto &Obj : Obj3d)
	{
		Obj->Draw();
	}

	// �p�[�e�B�N���}�l�[�W���̕`��
	particleManager->Draw();

	// 2D�I�u�W�F�N�g�̕`��
	for (auto &Obj : Obj2d)
	{
		Obj->Draw();
	}
}

//=============================================================================
// �f�o�b�O
//=============================================================================
void SceneResult::Debug()
{
#if _DEBUG_
	BeginDebugWindow("Result");

	DebugText("No1:%d No2:%d No3:%d No4:%d", SceneGame::GetResultData(0)->playerNo, SceneGame::GetResultData(1)->playerNo, SceneGame::GetResultData(2)->playerNo, SceneGame::GetResultData(3)->playerNo);
	DebugText("ResultTime\nNo1:%d No2:%d No3:%d No4:%d", SceneGame::GetResultData(0)->time, SceneGame::GetResultData(1)->time, SceneGame::GetResultData(2)->time, SceneGame::GetResultData(3)->time);

	EndDebugWindow("Result");

#endif
}