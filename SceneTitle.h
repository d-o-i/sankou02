//=============================================================================
//
// �^�C�g����ʏ��� [SceneTitle.h]
// Author : 
//
//=============================================================================
#ifndef _SCENETITLE_H_
#define _SCENETITLE_H_

#include "Scene.h"
class _2dobj;
class Map;
class Object3D;
class ParticleManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SceneTitle :
	public Scene
{
private:
	std::vector<_2dobj*> p2dObj;			// 2D�I�u�W�F�N�g�p�̃|�C���^
	std::vector <Object3D*>	object3d;		// 3D�I�u�W�F�N�g
	ParticleManager* particleManager;		// �p�[�e�B�N���}�l�[�W��
	Map* map;								// �}�b�v
	bool IsOption;							// �I�����t���O

public:
	SceneTitle();
	~SceneTitle();

	void Update(int SceneID);
	void Draw();
};

#endif
