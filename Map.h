//=============================================================================
//
// �}�b�v���� [Map.h]
// Author :  
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "Chip.h"
#include "Quadtree.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAP_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define MAP_SIZE_X		(500)								// �}�b�v�̉��̖���
#define MAP_SIZE_Y		(50)								// �}�b�v�̏c�̖���
#define START_POS		D3DXVECTOR3(50.0f, 0.0f, 0.0f)		// �X�^�[�g�n�_
//#define GOAL_POS		D3DXVECTOR3(500.0f, 0.0f, 0.0f)	// �S�[���n�_
#define GOAL_POS		D3DXVECTOR3(9800.0f, 0.0f, 0.0f)		// �S�[���n�_

// �}�b�v�`�b�v���W���擾�̎��
enum e_ChipPosType
{
	eLeftUp,	// �}�b�v�`�b�v�̍���̍��W 
	eLeftCenter,// �}�b�v�`�b�v�̍������̍��W 
	eRightUp,	// �}�b�v�`�b�v�̉E��̍��W 
	eCenter,	// �}�b�v�`�b�v�̒��S�̍��W 
	eCenterUp,	// �}�b�v�`�b�v�̒����̏�̍��W
};

// �t�B�[���h�I�u�W�F�N�g�̎��
enum e_ChipType
{
	eObjSpdup,		// �X�s�[�h�A�b�v
	eObjSpddown,	// �X�s�[�h�_�E��
	eObjNuma,		// ���i�X�s�[�h�_�E�����W�����v�̓_�E���j
	eObjJump,		// �����W�����v
	eObjDrain,		// �C���N�Q�[�W����
	eObjHeal,		// �C���N�Q�[�W����
	eObjItem,		// �A�C�e���擾
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Map
{
private:
	static std::vector<std::vector<int>>	maptbl;
	std::vector<Chip*>						MapChipVector;

	static std::vector<std::vector<int>>	objtbl;
	std::vector<Chip*>						ObjectChipVector;

public:
	Map();
	~Map();

	void Update();
	void Draw();

	void PaintCollider(QUADTREE *Quadtree, int NodeID);

	static int GetMapTbl(int MapX, int MapY);
	static int GetMapTbl(D3DXVECTOR3 Pos, int ChipDirection);
	static void GetMapChipXY(D3DXVECTOR3 Pos, int *MapX, int *MapY);
	static D3DXVECTOR3 GetMapChipPos(int x, int y, int PosType);
	static int GetObjTbl(int ObjX, int ObjY);
	static int GetObjTbl(D3DXVECTOR3 Pos, int ChipDirection);

	static void SetObjTbl(int ObjX, int ObjY, int texnum);
	std::vector<Chip*> GetObjectChip() { return ObjectChipVector; };

};

#endif
