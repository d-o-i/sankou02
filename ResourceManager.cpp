//=============================================================================
//
// ���\�[�X�}�l�[�W�� [ResouceManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResourceManager.h"

//*****************************************************************************
// ���O��Ԃ̊m��
//*****************************************************************************
using namespace std;

////=============================================================================
//// ���b�V���̓ǂݍ���
////=============================================================================
//void ResourceManager::LoadMesh(const char* tag, const char* path)
//{
//	string tagStr = string(tag);
//
//	// �^�O�̏d���m�F
//	if (modelPool.count(tagStr) != NULL)
//		SAFE_DELETE(modelPool[tagStr]);
//
//	// ���b�V���R���e�i�𐶐����ă��[�h
//	modelPool[tagStr] = new Model3D(tag, path);
//}
//
////=============================================================================
//// ���b�V���̊J��
////=============================================================================
//void ResourceManager::ReleaseMesh(const char* tag)
//{
//	string tagStr = string(tag);
//
//	// �^�O�̓o�^�m�F
//	if (modelPool.count(tagStr) == NULL)
//		return;
//
//	// ���b�V���R���e�i���폜
//	SAFE_DELETE(modelPool[tagStr]);
//}
//
////=============================================================================
//// ���b�V���̎擾
////=============================================================================
//bool ResourceManager::GetMesh(const char* tag, Model3D** pOut)
//{
//	string tagStr = string(tag);
//
//	// �o�^�m�F
//	if (modelPool.count(tagStr) == NULL)
//		return false;
//
//	// ���b�V���ւ̎Q�Ƃ��i�[
//	*pOut = modelPool[tagStr];
//	return true;
//}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void ResourceManager::LoadTexture(const char* tag, const char* path)
{
	string tagStr = string(tag);

	// �d���m�F
	if (texturePool.count(tagStr) != 0)
		SAFE_RELEASE(texturePool[tagStr]);

	// �ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, path, &texturePool[tagStr]);
}

//=============================================================================
// �e�N�X�`���̊J��
//=============================================================================
void ResourceManager::ReleaseTexture(const char* tag)
{
	string tagStr = string(tag);

	// �o�^�m�F
	if (texturePool.count(tagStr) == NULL)
		return;

	SAFE_RELEASE(texturePool[tagStr]);
}

//=============================================================================
// �e�N�X�`���̎擾
//=============================================================================
bool ResourceManager::GetTexture(const char* tag, LPDIRECT3DTEXTURE9 *pOut)
{
	string tagStr = string(tag);

	// �o�^�m�F
	if (texturePool.count(tagStr) == NULL)
		return false;

	*pOut = texturePool[tagStr];
	return true;
}

//=============================================================================
// ���\�[�X�̑S�폜
//=============================================================================
void ResourceManager::AllRelease()
{
	for (auto &pool : texturePool)
	{
		SAFE_RELEASE(pool.second)
	}
	texturePool.clear();
}