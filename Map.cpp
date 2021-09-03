//=============================================================================
//
// �}�b�v���� [Map.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Map.h"
#include "Input.h"
#include "MyLibrary.h"
#include "Collision.h"
#include "StageSelectBG.h"
#include "SceneManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �}�b�v�f�[�^
const char* MapFile[] = {
	("data/MAP/Gourmet_ground.csv"),
	("data/MAP/Ice_ground.csv"),
	("data/MAP/Lava_ground.csv"),
};

const char* ObjectFile[] = {
	("data/MAP/Gourmet_object.csv"),
	("data/MAP/Ice_object.csv"),
	("data/MAP/Lava_object.csv"),
};

//*****************************************************************************
// �����o�ϐ��̏�����
//*****************************************************************************
std::vector<std::vector<int>> Map::maptbl;
std::vector<std::vector<int>> Map::objtbl;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Map::Map()
{
	this->maptbl.reserve(MAP_SIZE_Y);
	for (int i = 0; i < MAP_SIZE_Y; i++)
	{
		vector<int> Vector_X;
		maptbl.push_back(Vector_X);
		maptbl.at(i).reserve(MAP_SIZE_X);
	}

	this->objtbl.reserve(MAP_SIZE_Y);
	for (int i = 0; i < MAP_SIZE_Y; i++)
	{
		vector<int> Vector_X;
		objtbl.push_back(Vector_X);
		objtbl.at(i).reserve(MAP_SIZE_X);
	}

	// csv�f�[�^�ǂݍ���
	if (GetScene() == nSceneGame)
	{
		ReadCsv(MapFile[StageSelectBG::GetStageSelect()], &this->maptbl);
		ReadCsv(ObjectFile[StageSelectBG::GetStageSelect()], &this->objtbl);
	}
	else if (GetScene() == nSceneTitle)
	{
		int r = rand() % 3;

		ReadCsv(MapFile[r], &this->maptbl);
		ReadCsv(ObjectFile[r], &this->objtbl);
	}

	for (int cntY = 0; cntY < MAP_SIZE_Y; cntY++)
	{
		for (int cntX = 0; cntX < MAP_SIZE_X; cntX++)
		{
			if (maptbl.at(cntY).at(cntX) >= 0)
			{
				MapChipVector.push_back(new Chip(cntX, cntY, maptbl.at(cntY).at(cntX), eMapChip));
			}

			if (objtbl.at(cntY).at(cntX) >= 0)
			{
				ObjectChipVector.push_back(new Chip(cntX, cntY, objtbl.at(cntY).at(cntX), eObjectChip));
			}
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Map::~Map()
{
	// �}�b�v�e�[�u���N���A
	maptbl.clear();
	ReleaseVector(maptbl);

	// �I�u�W�F�N�g�e�[�u���N���A
	objtbl.clear();
	ReleaseVector(objtbl);

	// �}�b�v�`�b�v�x�N�g���N���A
	for (auto &MapChip : this->MapChipVector)
	{
		SAFE_DELETE(MapChip);
	}
	MapChipVector.clear();
	ReleaseVector(MapChipVector);

	// �I�u�W�F�N�g�`�b�v�x�N�g���N���A
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->SetCnt(0);
		SAFE_DELETE(ObjectChip);
	}
	ObjectChipVector.clear();
	ReleaseVector(ObjectChipVector);
}

//=============================================================================
// �X�V
//=============================================================================
void Map::Update()
{
	for (auto &MapChip : this->MapChipVector)
	{
		MapChip->Update();
	}
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->Update();
	}
}

//=============================================================================
// �`��
//=============================================================================
void Map::Draw()
{
	for (auto &MapChip : this->MapChipVector)
	{
		MapChip->Draw();
	}
	for (auto &ObjectChip : this->ObjectChipVector)
	{
		ObjectChip->Draw();
	}
}

//=============================================================================
// �y�C���g�}�l�[�W���Ƃ̓����蔻��
//=============================================================================
void Map::PaintCollider(QUADTREE *Quadtree, int NodeID)
{
	std::vector<Paint*> CollisionList = Quadtree->GetObjectsAt(NodeID);

	// ���݂̃m�[�h�̓I�u�W�F�N�g���Ȃ�
	if (CollisionList.empty())
	{
		return;
	}

	for (auto &BlackPaint : CollisionList)
	{
		// �g�p���Ă���A�J���[�����̂��̂�T��
		if (!BlackPaint->GetUse() || BlackPaint->GetPaintColor() != BlackInkColor)
			continue;

		for (auto &Obj : ObjectChipVector)
		{
			// �g�p���Ă��Ȃ��܂��͂��łɔ��]���Ă���ꍇ�͔��肵�Ȃ�
			if (!Obj->GetUse() || Obj->GetReverse())
				continue;

			if (HitSphere(BlackPaint->GetPos(), Obj->GetPos(), Paint::GetPaintRadius(), CHIP_SIZE))
			{
				BlackPaint->SetUse(false);
				// �t�B�[���h�I�u�W�F�N�g�𔽓]������
				Obj->ReverseTexture();
				Obj->SetReverse(true);
				break;
			}
		}
	}
}

//=============================================================================
// ���݂̍��W���瑫���}�b�v�`�b�v��XY���擾����
//=============================================================================
void Map::GetMapChipXY(D3DXVECTOR3 Pos, int *MapX, int *MapY)
{
	*MapX = (int)((Pos.x + CHIP_SIZE / 2) / CHIP_SIZE);
	*MapY = abs((int)((Pos.y - CHIP_SIZE / 2) / CHIP_SIZE));
}

//=============================================================================
// �}�b�v�`�b�v�̍��W���擾����
//=============================================================================
D3DXVECTOR3 Map::GetMapChipPos(int x, int y, int PosType)
{
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 2D��Y���W��3D��Y���W�̕����͋t
	switch (PosType)
	{
	case eLeftUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE - CHIP_SIZE / 2, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	case eLeftCenter:
		Pos = D3DXVECTOR3(x * CHIP_SIZE - CHIP_SIZE / 2, -(y * CHIP_SIZE), 0.0f);
		break;
	case eRightUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE + CHIP_SIZE / 2, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	case eCenter:
		Pos = D3DXVECTOR3(x * CHIP_SIZE, -(y * CHIP_SIZE), 0.0f);
		break;
	case eCenterUp:
		Pos = D3DXVECTOR3(x * CHIP_SIZE, -(y * CHIP_SIZE - CHIP_SIZE / 2), 0.0f);
		break;
	default:
		break;
	}

	return Pos;
}

//=============================================================================
// �}�b�v�`�b�vXY����}�b�v�e�[�u���̐��l���擾����
//=============================================================================
int Map::GetMapTbl(int MapX, int MapY)
{
	if (MapX < 0 ||
		MapX >= MAP_SIZE_X ||
		MapY < 0 ||
		MapY >= MAP_SIZE_Y)
	{
		return -1;
	}
	else
	{
		return maptbl.at(MapY).at(MapX);
	}
}

//=============================================================================
// ���W����}�b�v�e�[�u���̐��l���擾����
//=============================================================================
// ������
// ������	���FCenterChip
// ������
int Map::GetMapTbl(D3DXVECTOR3 Pos, int ChipDirection)
{
	int x = 0;
	int y = 0;

	GetMapChipXY(Pos, &x, &y);

	switch (ChipDirection)
	{
	case eCenterUp:
		// �����̏�̃`�b�v
		y--;
		break;
	default:
		break;
	}

	return GetMapTbl(x, y);
}

//=============================================================================
// �I�u�W�F�N�g�`�b�vXY����I�u�W�F�N�g�e�[�u���̐��l���擾����
//=============================================================================
int Map::GetObjTbl(int ObjX, int ObjY)
{
	if (ObjX < 0 ||
		ObjX >= MAP_SIZE_X ||
		ObjY < 0 ||
		ObjY >= MAP_SIZE_Y)
	{
		return -1;
	}
	else
	{
		return objtbl.at(ObjY).at(ObjX);
	}
}

//=============================================================================
// ���W����}�b�v�e�[�u���̐��l���擾����
//=============================================================================
// ������
// ������	���FCenterChip
// ������
int Map::GetObjTbl(D3DXVECTOR3 Pos, int ChipDirection)
{
	int x = 0;
	int y = 0;

	GetMapChipXY(Pos, &x, &y);

	switch (ChipDirection)
	{
	case eCenterUp:
		// �����̏�̃`�b�v
		y--;
		break;
	default:
		break;
	}

	return GetObjTbl(x, y);
}

void Map::SetObjTbl(int ObjX, int ObjY, int texnum)
{
	objtbl.at(ObjY).at(ObjX) = texnum;
}