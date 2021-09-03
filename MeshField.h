//=============================================================================
//
// ���b�V���n�ʂ̏��� [MeshField.cpp]
// Author :  
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "Object3D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class MeshField :
	public Object3D
{
private:
	int NumBlockX, NumBlockZ;				// �u���b�N��
	int NumVertex;							// �����_��	
	int NumVertexIndex;						// ���C���f�b�N�X��
	int NumPolygon;							// ���|���S����
	float BlockSizeX, BlockSizeZ;			// �u���b�N�T�C�Y

public:
	MeshField();
	~MeshField();

	void Update();
	void Draw();

};

#endif
