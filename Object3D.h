//=============================================================================
//
// 3D�I�u�W�F�N�g�p���N���X [Object3D.h]
// Author :  
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Object3D
{
protected:
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		D3DTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	D3DIdxBuff;	// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	D3DXVECTOR3				pos;		// ���W
	D3DXVECTOR3				rot;		// ��]
	D3DXVECTOR3				scl;		// �X�P�[��
	D3DXCOLOR				col;		// �F
	D3DXVECTOR3				move;		// �ړ���
	bool					use;

public:
	Object3D();
	virtual ~Object3D();

	// �������z�֐�
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif
