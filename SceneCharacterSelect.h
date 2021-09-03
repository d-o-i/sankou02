//=============================================================================
//
// キャラクターセレクト画面処理 [SceneCharacterSelect.h]
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
// クラス定義
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

	static int GetSelectCharacter(int no);		// 選択したキャラクターの番号
	static bool GetAI(int no);
};

#endif
