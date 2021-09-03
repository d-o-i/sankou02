//=============================================================================
//
// リソースマネージャ [ResouceManager.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResourceManager.h"

//*****************************************************************************
// 名前空間の確保
//*****************************************************************************
using namespace std;

////=============================================================================
//// メッシュの読み込み
////=============================================================================
//void ResourceManager::LoadMesh(const char* tag, const char* path)
//{
//	string tagStr = string(tag);
//
//	// タグの重複確認
//	if (modelPool.count(tagStr) != NULL)
//		SAFE_DELETE(modelPool[tagStr]);
//
//	// メッシュコンテナを生成してロード
//	modelPool[tagStr] = new Model3D(tag, path);
//}
//
////=============================================================================
//// メッシュの開放
////=============================================================================
//void ResourceManager::ReleaseMesh(const char* tag)
//{
//	string tagStr = string(tag);
//
//	// タグの登録確認
//	if (modelPool.count(tagStr) == NULL)
//		return;
//
//	// メッシュコンテナを削除
//	SAFE_DELETE(modelPool[tagStr]);
//}
//
////=============================================================================
//// メッシュの取得
////=============================================================================
//bool ResourceManager::GetMesh(const char* tag, Model3D** pOut)
//{
//	string tagStr = string(tag);
//
//	// 登録確認
//	if (modelPool.count(tagStr) == NULL)
//		return false;
//
//	// メッシュへの参照を格納
//	*pOut = modelPool[tagStr];
//	return true;
//}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void ResourceManager::LoadTexture(const char* tag, const char* path)
{
	string tagStr = string(tag);

	// 重複確認
	if (texturePool.count(tagStr) != 0)
		SAFE_RELEASE(texturePool[tagStr]);

	// 読み込み
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, path, &texturePool[tagStr]);
}

//=============================================================================
// テクスチャの開放
//=============================================================================
void ResourceManager::ReleaseTexture(const char* tag)
{
	string tagStr = string(tag);

	// 登録確認
	if (texturePool.count(tagStr) == NULL)
		return;

	SAFE_RELEASE(texturePool[tagStr]);
}

//=============================================================================
// テクスチャの取得
//=============================================================================
bool ResourceManager::GetTexture(const char* tag, LPDIRECT3DTEXTURE9 *pOut)
{
	string tagStr = string(tag);

	// 登録確認
	if (texturePool.count(tagStr) == NULL)
		return false;

	*pOut = texturePool[tagStr];
	return true;
}

//=============================================================================
// リソースの全削除
//=============================================================================
void ResourceManager::AllRelease()
{
	for (auto &pool : texturePool)
	{
		SAFE_RELEASE(pool.second)
	}
	texturePool.clear();
}