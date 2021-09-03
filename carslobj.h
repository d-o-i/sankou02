//=============================================================================
//
// バトル画面フレーム表示処理 [Carslobj.h]
// Author : 
//
//=============================================================================
#ifndef _CARSLOBJ_H_
#define _CARSLOBJ_H_

#include "_2dobj.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_CARSLOBJ	_T("data/texture/charasl_obj.png")			// フレーム用画像
#define TEXTURE_CARSLOBJ2	_T("data/texture/charasl_obj2.png")			// フレーム用画像
#define TEXTURE_CARSLOBJ3	_T("data/texture/charasl_obj3.png")			// フレーム用画像
#define TEXTURE_CARSLOBJ4	_T("data/texture/charasl_obj4.png")			// フレーム用画像
#define CARSLOBJ_SIZE		D3DXVECTOR3(114.0f, 114.0f, 0.0f)		// テクスチャサイズ
#define AO					(0)
#define AKA					(1)
#define MIDORI				(2)
#define KI					(3)
#define CARSLOBJ_POS01		D3DXVECTOR3(400.0f, 300.0f, 0.0f)
#define CARSLOBJ_POS02		D3DXVECTOR3(550.0f, 300.0f, 0.0f)
#define CARSLOBJ_POS03		D3DXVECTOR3(700.0f, 300.0f, 0.0f)
// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標

} VERTEX_2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Carslobj :
	public _2dobj {
public:
	bool  player = false;

	Carslobj(D3DXVECTOR3 _pos, const char *texno);
	~Carslobj();

	// オーバーライド関数
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex();

};
int Getpnum();
void Setpnum(int no);
bool Getch();
void Setch(bool ch);

#endif