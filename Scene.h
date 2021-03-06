//=============================================================================
//
// シーン管理の抽象インターフェースクラス [Scene.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ゲーム結果
typedef struct {
	int playerNo;	// その順位のプレイヤー番号
	DWORD time;		// ゴール時間
}ResultData;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Update(int SceneID) = 0;
	virtual void Draw() = 0;
};

#endif
