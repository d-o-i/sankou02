//=============================================================================
//
// キャラクターセレクトUI [CharSelectUI.h]
// Author :  
//
//=============================================================================
#ifndef _CharSelectUI_H_
#define _CharSelectUI_H_


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CharSelectUI
{
private:
	LPDIRECT3DTEXTURE9	PlayerNumTex;
	LPDIRECT3DTEXTURE9	ModelTex;
	LPDIRECT3DTEXTURE9	CPUIconTex;
	Vertex2D			PlayerNumVtx[NUM_VERTEX];	// 頂点情報格納構造体
	Vertex2D			ModelVtx[NUM_VERTEX];		
	Vertex2D			CPUIconVtx[NUM_VERTEX];		
	D3DXVECTOR3			Pos;						// 座標
	int					PlayerNo;
	bool				SelectOver = false;			// 使用フラグ
	bool				AIUse = false;

	void MakeVertex();

public:
	CharSelectUI(int PlayerNo, bool AIUse);
	~CharSelectUI();

	// オーバーライド関数
	void Update();
	void Draw();

	void SetCharTexture(int CharacterNo);
};


#endif