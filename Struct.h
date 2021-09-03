//=============================================================================
//
// �\���̒�`�p [Struct.h]
// Author :  
//
//=============================================================================
#ifndef _STRUCT_H_
#define _STRUCT_H_

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
struct Vertex2D
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
	Vertex2D()
	{
		this->rhw = 1.0f;
		this->diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
};

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
struct Vertex3D
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
	Vertex3D()
	{
		this->diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
};

// �O�p�`�|���S���p�̍\����
struct TriangleStr
{
	D3DXVECTOR3 pos0;
	D3DXVECTOR3 pos1;
	D3DXVECTOR3 pos2;
};

struct Int2D
{
	int x;
	int y;
	Int2D() {};
	Int2D(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Float2D
{
	float x;
	float y;
	Float2D() {};
	Float2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

#endif
