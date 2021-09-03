//=============================================================================
//
// 3Dオブジェクト用基底クラス [Object3D.h]
// Author :  
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Object3D
{
protected:
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		D3DTexture;	// テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9	D3DIdxBuff;	// インデックスバッファインターフェースへのポインタ
	D3DXVECTOR3				pos;		// 座標
	D3DXVECTOR3				rot;		// 回転
	D3DXVECTOR3				scl;		// スケール
	D3DXCOLOR				col;		// 色
	D3DXVECTOR3				move;		// 移動量
	bool					use;

public:
	Object3D();
	virtual ~Object3D();

	// 純粋仮想関数
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif
