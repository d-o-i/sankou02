//=============================================================================
//
// ���\�[�X�}�l�[�W�� [ResourceManager.h]
// Author :  
//
//=============================================================================
#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "BaseSingleton.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ResourceManager :
	public BaseSingleton<ResourceManager>
{
	// BaseSingleton�N���X��Inctance���Ăяo����悤��
	friend class BaseSingleton<ResourceManager>;

public:
	ResourceManager() {};
	~ResourceManager() {};

	// �ǂݍ��݁A�J������
	// ���b�V���R���e�i
	//void LoadMesh(const char* tag, const char* path);
	//void ReleaseMesh(const char* tag);

	// 2D�e�N�X�`����3D�|���S��
	void LoadTexture(const char* tag, const char* path);
	void ReleaseTexture(const char* tag);

	// �Q�Ə���
	//bool GetMesh(const char* tag, Model3D **pOut);
	bool GetTexture(const char* tag, LPDIRECT3DTEXTURE9* pOut);

	// �S���\�[�X�J������
	void AllRelease();

private:
	//std::map<std::string, Model3D*> modelPool;				// ���f������ۑ����Ă����ꏊ
	std::map<std::string, LPDIRECT3DTEXTURE9> texturePool;	// �e�N�X�`������ۑ����Ă����ꏊ
};

#endif
