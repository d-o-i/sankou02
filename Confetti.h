//=============================================================================
//
// ������ [Confetti.h]
// Author :  
//
//=============================================================================
#ifndef _CONFETTI_H_
#define _CONFETTI_H_

#include "Billboard.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Confetti :
	public Billboard
{
private:
	bool						use;			// �g�p���Ă��邩�ǂ���
	int							texNo;			// �g�p����e�N�X�`���ԍ�
	int							moveCnt;		// ����������^�C�~���O
	static LPDIRECT3DTEXTURE9	D3DTexture;

	HRESULT MakeVertex();

public:
	Confetti();
	~Confetti();

	void Update();
	void Draw();

	bool GetUse() { return use; };

	static void ReleaseTexture() { SAFE_RELEASE(D3DTexture); };

};

#endif
