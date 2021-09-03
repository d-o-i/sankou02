//=============================================================================
//
// �L�����N�^�[�Z���N�gUI [CharSelectUI.h]
// Author :  
//
//=============================================================================
#ifndef _CharSelectUI_H_
#define _CharSelectUI_H_


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CharSelectUI
{
private:
	LPDIRECT3DTEXTURE9	PlayerNumTex;
	LPDIRECT3DTEXTURE9	ModelTex;
	LPDIRECT3DTEXTURE9	CPUIconTex;
	Vertex2D			PlayerNumVtx[NUM_VERTEX];	// ���_���i�[�\����
	Vertex2D			ModelVtx[NUM_VERTEX];		
	Vertex2D			CPUIconVtx[NUM_VERTEX];		
	D3DXVECTOR3			Pos;						// ���W
	int					PlayerNo;
	bool				SelectOver = false;			// �g�p�t���O
	bool				AIUse = false;

	void MakeVertex();

public:
	CharSelectUI(int PlayerNo, bool AIUse);
	~CharSelectUI();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();

	void SetCharTexture(int CharacterNo);
};


#endif