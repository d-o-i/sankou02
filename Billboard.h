//=============================================================================
//
// ビルボード用クラス [Billboard.h]
// Author :  
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "Struct.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Billboard
{
protected:
	LPDIRECT3DTEXTURE9		D3DTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				rot;			// 回転
	D3DXVECTOR3				scl;			// スケール
	D3DXCOLOR				col;			// 色
	D3DXVECTOR3				move;			// 移動量
	float					width;			// 幅
	float					height;			// 高さ

public:
	Billboard();
	virtual ~Billboard();

	// 純粋仮想関数
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif
