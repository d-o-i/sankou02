//=============================================================================
//
// �L�����N�^�[�Z���N�g�J�[�\�� [CursorObj.h]
// Author :  
//
//=============================================================================
#ifndef _CURSOROBJ_H_
#define _CURSOROBJ_H_

#include "_2dobj.h"

//*****************************************************************************
// �}�N����`
#define TEXTURE_CURSOROBJ	("data/TEXTURE/CharSelectCursor.png")		// �t���[���p�摜
#define TEXTURE_CURSOROBJ2	("data/TEXTURE/CpuLogo.png")				// �t���[���p�摜
#define CURSOROBJ_DIVIDE_X	(4)											// ������
#define CURSOROBJ_MAX		(4)											// 1�v���C���[������̃J�[�\����

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CursorObj :
	public _2dobj
{
private:
	static LPDIRECT3DTEXTURE9	D3DTexture;	// �e�N�X�`���̃|�C���^
	static LPDIRECT3DTEXTURE9	D3DTexture2;	// �e�N�X�`���̃|�C���^

	int playerNo;	// �Q�Ƃ���v���C���[�ԍ�
	int cursorNo;	// ���W���Q�Ƃ���ԍ�
	int selectNo;	// �I�������L�����N�^�[�̔ԍ�
	
	HRESULT MakeVertex();
	HRESULT MakeVert();
	void SetTexture();
	void SetTex();
	void SetVertex();
	void SetVert();

	void Move();	// �J�[�\���̈ړ�

public:
	CursorObj(int playerNo, int cursorNo);
	~CursorObj();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	void Draw2();

	int GetCursorNo() { return cursorNo; };
	int GetSelectNo() { return selectNo; };

};

#endif