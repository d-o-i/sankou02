//=============================================================================
//
// �r���{�[�h�p�N���X [Billboard.h]
// Author :  
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "Struct.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Billboard
{
protected:
	LPDIRECT3DTEXTURE9		D3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				rot;			// ��]
	D3DXVECTOR3				scl;			// �X�P�[��
	D3DXCOLOR				col;			// �F
	D3DXVECTOR3				move;			// �ړ���
	float					width;			// ��
	float					height;			// ����

public:
	Billboard();
	virtual ~Billboard();

	// �������z�֐�
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif
