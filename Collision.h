//=============================================================================
//
// �R���W����[Collision.h]
// Author : 
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Quadtree.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool HitCheckBB(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR2 sizeA, D3DXVECTOR2 sizeB);	// ��`�̓����蔻��
bool HitSphere(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, float Range1, float Range2);				// ���̓����蔻��

// �y�C���g�V�X�e�����m�̓����蔻��
//void HitCheckSToS(QUADTREE *Quadtree, int NodeID);
void HitCheckSToS(std::vector<Paint*> *CollisionList, int PlayerNo);

float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);											// ����
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);						// �O��
int hitCheck(D3DXVECTOR3 *OutPos, TriangleStr tri, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		// �|���S���Ɛ����̓����蔻��

#endif

