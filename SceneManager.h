//=============================================================================
//
// �V�[���}�l�[�W�� [SceneManager.h]
// Author :  
//
//=============================================================================
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Scene.h"

//�V�[���J�ڔԍ�
enum SceneNum
{
	nSceneTitle,			// �^�C�g��
	nSceneTutorial,			// �`���[�g���A��
	nSceneStageSelect,		// �X�e�[�W�Z���N�g
	nSceneCharacterSelect,	// �L�����N�^�[�Z���N�g
	nSceneGame,				// �Q�[��
	nSceneResult,			// ���U���g
	nSceneExit				// �Q�[���I��
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SceneManager
{
public:
	SceneManager(HINSTANCE hInstance, HWND hWnd);
	~SceneManager();

	void Update();
	void Draw();

	void LoadResource();
};

int GetScene();									// ���݂̃Q�[���V�[�����擾����
//void SetScene(Scene *NewScene, int _scene);		// �Q�[���V�[����ύX����
void SetScene(int _scene);		// �Q�[���V�[����ύX����

#endif
