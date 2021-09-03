//=============================================================================
//
// 2D用オブジェクト処理 [_2dobj.h]
// Author :  
//
//=============================================================================
#ifndef _2DOBJ_H_
#define _2DOBJ_H_

#include "main.h"
//*****************************************************************************
// 2D用オブジェクト基底クラス定義
//*****************************************************************************
class _2dobj //クラス名
{
	//   private:　    不可触　継承不可
	//   protected:    不可触　継承可
	//   public:       可触　　継承可
protected:
	// メンバ変数
	LPDIRECT3DTEXTURE9	D3DTexture;					// テクスチャのポインタ
	Vertex2D			vertexWk[NUM_VERTEX];		// 頂点情報格納構造体
	D3DXVECTOR3			pos;						// 座標
	D3DXVECTOR3			size;						// サイズ
	int					PatternAnim;				// アニメーションパターン
	bool				use;						// 使用フラグ

public:
	// メンバ関数
	_2dobj();
	virtual ~_2dobj();
		

	// 仮想関数
	virtual void Update() {};						// 更新
	virtual void Draw() = 0;						// 描画

};

#endif