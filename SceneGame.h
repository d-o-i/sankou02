//=============================================================================
//
// �Q�[����ʏ��� [SceneGame.h]
// Author :  
//
//=============================================================================
#ifndef _SCENEGAME_H_
#define _SCENEGAME_H_

#include "Scene.h"

// �|�C���^�Ƃ��Ď擾����K�v�̂���N���X
#include "Map.h"
#include "_2dobj.h"
#include "Quadtree.h"
#include "Player.h"
#include "PaintGroup.h"
#include "PaintManager.h"
#include "EffectManager.h"
#include "Object3D.h"
#include "Timer.h"
#include "ParticleManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define START_FRAME (240)	// �X�^�[�g����܂ł̃^�C�}�[


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SceneGame :
	public Scene
{
private:
	std::vector<_2dobj*>		UIObject;	// UI
	std::vector <Object3D*>		object3d;	// 3D�I�u�W�F�N�g
	Map				*pMap;					// �}�b�v
	Player			*pPlayer[PLAYER_MAX];	// �v���C���[
	QUADTREE		*Quadtree = nullptr;	// �l����
	PaintGroup		*paintGroup = nullptr;	// �y�C���g�O���[�v
	EffectManager	*pEffectManager;		// �G�t�F�N�g�Ǘ�
	Timer			*pTimer;				// �^�C�}�[
	ParticleManager *particleManager;		// �p�[�e�B�N���}�l�[�W��

	int				startframe;				// �J�n�J�E���g�_�E��
	bool			result;					// �I���t���O
	static ResultData data[PLAYER_MAX];		// ����
	static int TheLastPlayer;				// �ŉ��ʂ̃v���C���[

	void Start();
	void Collision();
	void CheckResult();
	void InsertResult(int pNo);
	void Debug();

public:
	SceneGame();
	~SceneGame();
	void Update(int SceneID);
	void Draw();
	static ResultData *GetResultData(int rank);
	static int GetTheLastPlayer(void) { return SceneGame::TheLastPlayer; };
};


#endif
