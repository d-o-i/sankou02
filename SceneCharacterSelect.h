//=============================================================================
//
// �L�����N�^�[�Z���N�g��ʏ��� [SceneCharacterSelect.h]
// Author :  
//
//=============================================================================
#ifndef _SCENECHARACTERSELECT_H_
#define _SCENECHARACTERSELECT_H_

#include "Scene.h"
#include "_2dobj.h"
#include "CursorObj.h"
#include "Player.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SceneCharacterSelect :
	public Scene
{
private:
	std::vector<_2dobj*> p2dobj;
	CursorObj *pCursor[PLAYER_MAX][CURSOROBJ_MAX];
	static int SelectCharacter[PLAYER_MAX];

public:
	SceneCharacterSelect();
	~SceneCharacterSelect();

	void Update(int SceneID);
	void Draw();

	static int GetSelectCharacter(int no);		// �I�������L�����N�^�[�̔ԍ�
	static bool GetAI(int no);
};

#endif
