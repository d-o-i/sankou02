//=============================================================================
//
// カーソル表示 [Cursor.h]
// Author :  
//
//=============================================================================

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "_2dobj.h"
#include "CharacterAI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CURSOR_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)				// サイズ
#define CURSOR_DIVIDE_X	(4)										// 横分割
#define CURSOR_DIVIDE_Y	(2)										// 縦分割
#define CURSOR_PATTERN	(CURSOR_DIVIDE_X * CURSOR_DIVIDE_Y)		// 分割数
#define CURSOR_FIRST_POS	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
#define CURSOR_SPEED	(30.0f)									// 動くスピード

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Cursor : public _2dobj
{
private:
	int		ctrlNum;			// 操作するコントローラ番号
	float	vec = 0.0f;			// ジョイスティックのベクトルを1/1000にして保存(1.0f-0.0f)
	float	moveX = 0.0f;		// ジョイスティックのX値を1/1000にして保存(1.0f-0.0f)
	float	moveY = 0.0f;		// ジョイスティックのY値を1/1000にして保存(1.0f-0.0f)
	//static LPDIRECT3DTEXTURE9 D3DTexture;					// テクスチャのポインタ

	// AI用
	CharacterAI		*AIptr = nullptr;
	bool			AIUse = false;
	D3DXVECTOR3		WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	void PaintPath(void);					// インクで通れる道を作る
	void DeletePath(void);					// 他のプレイヤーのペイントを削除
	void PaintObjChip(void);				// オブジェクトチップをペイントする

	HRESULT MakeVertex();				// 頂点の作成
	void SetTexture();					// テクスチャ座標の設定
	void SetVertex();					// 頂点座標の設定
	void KeyMove();						// キーボード操作
	void PadMove();						// コントローラ操作
	void Move();						// 操作
	void CalWorldPos();

public:
	Cursor(int PlayerNo, bool AIUse, CharacterAI *AIptr);
	~Cursor();

	// 更新
	void Update();
	// 描画
	void Draw();	
	// インクを切り替え
	void ChangeInk();						
	// テクスチャの開放
	//static void ReleaseTexture(void) { SAFE_RELEASE(Cursor::D3DTexture); };

	// カーソルの座標を取得
	D3DXVECTOR3 GetPos(void) { return this->pos; };
	D3DXVECTOR3 GetWorldPos(void) { return this->WorldPos; };
};

#endif
