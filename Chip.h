//=============================================================================
//
// チップ処理 [Chip.h]
// Author :  
//
//=============================================================================
#ifndef _CHIP_H_
#define _CHIP_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHIP_SIZE		(20.0f)								// マップチップ一枚のサイズ


enum eChipType
{
	eMapChip,
	eObjectChip,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Chip
{
private:
	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	D3DXVECTOR3				scl;
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		D3DTexture;			// テクスチャへのポインタ
	bool use;
	int ChipType;
	bool reverse;
	int cnt;
	int texnum;
	int mapX, mapY;

	HRESULT MakeVertex();
	void CheckOnCamera();
	void SetTexture();

public:
	Chip(int x, int y, int _texnum, int ChipType);
	~Chip();

	void Update();
	void Draw();

	void ReverseTexture();

	D3DXVECTOR3 GetPos() { return pos; };
	bool GetUse() { return use; };
	bool GetReverse() { return reverse; };
	int GetTextureNum() { return texnum; };

	void SetReverse(bool _reverse) { reverse = _reverse; };
	void SetCnt(int num) { cnt = num; };
};

#endif
