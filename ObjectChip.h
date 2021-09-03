//=============================================================================
//
// フィールドオブジェクトチップ処理 [ObjectChip.h]
// Author :  
//
//=============================================================================
#ifndef _OBJECTCHIP_H_
#define _OBJECTCHIP_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECTCHIP_TEXTURE	("data/MAP/mapchip_object.png")
#define OBJECTCHIP_DIVIDE_X	(7)
#define OBJECTCHIP_DIVIDE_Y	(1)

// フィールドオブジェクトの種類
#define OBJ_NUM_SPDUP		(0)	// 加速板
#define OBJ_NUM_SPDDOWN		(1)	// 減速板
#define OBJ_NUM_NUMA		(2)	// 沼地（減速）
#define OBJ_NUM_JUMP		(3)	// ジャンプ台
#define OBJ_NUM_DRAIN		(4)	// ドレインフロア
#define OBJ_NUM_HEAL		(5)	// 回復板
#define OBJ_NUM_ITEM		(6)	// アイテム

//*****************************************************************************
// クラス定義
//*****************************************************************************
class ObjectChip
{
private:
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	D3DXVECTOR3	scl;
	static LPDIRECT3DTEXTURE9 D3DTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;	// 頂点バッファへのポインタ
	bool use;
	bool reverse;
	int texnum;
	int reversecnt;

	HRESULT MakeVertex();
	void SetTexture();
	void CheckOnCamera();

public:
	ObjectChip(int x, int y, int _texnum);
	~ObjectChip();

	void Update();
	void Draw();

	void ReverseTexture();	// テクスチャを反転させる

	D3DXVECTOR3 GetPos() { return pos; };
	bool GetUse() { return use; };
	bool GetReverse() { return reverse; };
	int GetTextureNum() { return texnum; };

	void SetReverse(bool _reverse) { reverse = _reverse; };
};

#endif
