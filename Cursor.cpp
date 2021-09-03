//=============================================================================
//
// カーソル表示 [Cursor.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Cursor.h"
#include "Camera.h"
#include "Input.h"
#include "Collision.h"
#include "MyLibrary.h"
#include "DebugWindow.h"
#include "CharacterAI.h"
#include "ResourceManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CURSOR_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)				// サイズ
#define CURSOR_DIVIDE_X	(4)										// 横分割
#define CURSOR_DIVIDE_Y	(2)										// 縦分割
#define CURSOR_FIRST_POS	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
#define CURSOR_SPEED	(30.0f)									// 動くスピード

//=============================================================================
// コンストラクタ
//=============================================================================
Cursor::Cursor(int PlayerNo, bool AIUse, CharacterAI *AIptr)
{
	ResourceManager::Instance()->GetTexture("Cursor", &D3DTexture);

	use = true;
	pos = CURSOR_FIRST_POS;
	PatternAnim = ctrlNum = PlayerNo;
	// AIポインタの設定
	if (AIUse)
	{
		this->AIUse = true;
		this->AIptr = AIptr;
	}
	else
	{
		this->AIUse = false;
		this->AIptr = nullptr;
	}

	// 頂点情報の作成
	MakeVertex();
}

Cursor::Cursor(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("Cursor", &D3DTexture);

	use = true;
	pos = CURSOR_FIRST_POS;
	PatternAnim = ctrlNum = PlayerNo;
	this->AIUse = false;
	this->AIptr = nullptr;

	// 頂点情報の作成
	MakeVertex();
}

//=============================================================================
// デストラクタ
//=============================================================================
Cursor::~Cursor()
{
	D3DTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void Cursor::Update()
{
	if (use)
	{
		// 操作
		Move();

		// スクリーン座標からワールド座標に変換
		CalWorldPos();

		// 頂点座標の設定
		SetVertex();
	}

#if 0
#if _DEBUG
	ImGui::SetNextWindowPos(ImVec2(5, 300), ImGuiSetCond_Once);

	BeginDebugWindow("Cursor");

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Cursor Postion"))
	{
		if (ImGui::TreeNode("ScreenPos"))
		{
			DebugText("Pos X:%.2f\nPos Y:%.2f\n", pos.x, pos.y);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("WorldPos"))
		{
			DebugText("X:%.2f\nY:%.2f\nZ:%.2f\n", WorldPos.x, WorldPos.y, WorldPos.z);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	EndDebugWindow("Cursor");

#endif
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void Cursor::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		Device->SetTexture(0, D3DTexture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

	// αテストを無効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT Cursor::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定
	SetVertex();

	// rhwの設定
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void Cursor::SetTexture()
{
	int x = PatternAnim % CURSOR_DIVIDE_X;
	int y = PatternAnim / CURSOR_DIVIDE_X;
	float sizeX = 1.0f / CURSOR_DIVIDE_X;
	float sizeY = 1.0f / CURSOR_DIVIDE_Y;

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void Cursor::SetVertex()
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y - CURSOR_SIZE.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + CURSOR_SIZE.x, pos.y - CURSOR_SIZE.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + CURSOR_SIZE.x, pos.y, 0.0f);
}

//=============================================================================
// 操作
//=============================================================================
void Cursor::Move()
{
	if (!AIUse)
	{
		KeyMove();	// キーボード操作
		PadMove();	// コントローラ操作
	}
	else
	{
		// ペイントルートを描く
		if (AIptr->GetCursorState() == ePaintPath)
		{
			PaintPath();
		}
		// 他のプレイヤーのルートを消す
		else if (AIptr->GetCursorState() == eUseBlackPaint)
		{
			DeletePath();
		}
		// オブジェクトチップを反転させる
		else if (AIptr->GetCursorState() == ePaintObjChip)
		{
			PaintObjChip();
		}

#if _DEBUG
		//KeyMove();	// キーボード操作
#endif
	}
}

//=============================================================================
// カーソルの切り替え
//=============================================================================
void Cursor::ChangeInk()
{
	// テクスチャとプレイヤーのインクの切り替え
	// カラー→黒
	if (PatternAnim == ctrlNum)
	{
		PatternAnim = ctrlNum + CURSOR_DIVIDE_X;
	}
	// 黒→カラー
	else
	{
		PatternAnim = ctrlNum;
	}

	//テクスチャ座標をセット
	SetTexture();
}

//=============================================================================
// キーボード操作
//=============================================================================
void Cursor::KeyMove()
{
	// 上下
	if (GetKeyboardPress(DIK_W))
	{
		pos.y -= CURSOR_SPEED;

		// 画面外判定
		pos.y = max(pos.y, 0.0f + CURSOR_SIZE.y);
	}
	else if (GetKeyboardPress(DIK_S))
	{
		pos.y += CURSOR_SPEED;

		// 画面外判定
		pos.y = min(pos.y, SCREEN_HEIGHT);
	}

	// 左右
	if (GetKeyboardPress(DIK_D))
	{
		pos.x += CURSOR_SPEED;

		// 画面外判定
		pos.x = min(pos.x, SCREEN_WIDTH - CURSOR_SIZE.x);
	}
	else if (GetKeyboardPress(DIK_A))
	{
		pos.x -= CURSOR_SPEED;

		// 画面外判定
		pos.x = max(pos.x, 0.0f);
	}
}

//=============================================================================
// コントローラ操作
//=============================================================================
void Cursor::PadMove()
{
	vec = GetJoyStickVec(ctrlNum) / 1000.f;
	moveX = (float)GetJoyStickLeftX(ctrlNum) / 1000.0f;
	moveY = (float)GetJoyStickLeftY(ctrlNum) / 1000.0f;

	pos.x += vec * moveX * CURSOR_SPEED;
	pos.y += vec * moveY * CURSOR_SPEED;

	// 画面外に出た場合、古い座標に戻す
	//pos.x = clamp(pos.x, 0.0f, SCREEN_WIDTH - CURSOR_SIZE.x);
	//pos.y = clamp(pos.y, 0.0f, SCREEN_HEIGHT - CURSOR_SIZE.y);
	pos.x = clamp(pos.x, 0.0f, SCREEN_WIDTH - CURSOR_SIZE.x);
	pos.y = clamp(pos.y, CURSOR_SIZE.y, (float)SCREEN_HEIGHT);
}

//=============================================================================
// ワールド座標を計算する
//=============================================================================
void Cursor::CalWorldPos()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *camerawk = GetCamera();
	D3DXMATRIX ViewMtx, ProjMtx;

	Device->GetTransform(D3DTS_VIEW, &ViewMtx);
	Device->GetTransform(D3DTS_PROJECTION, &ProjMtx);

	// カーソルのスクリーン座標をワールド座標へ変換して座標をセット
	// スクリーン座標とXZ平面のワールド座標交点算出
	D3DXVECTOR3 OutPos1, OutPos2, SetPos;
	CalcScreenToWorld(&OutPos1, (int)pos.x, (int)pos.y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &ViewMtx, &ProjMtx);
	CalcScreenToWorld(&OutPos2, (int)pos.x, (int)pos.y, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &ViewMtx, &ProjMtx);

	// 判定用三角形ポリゴン
	TriangleStr triPos1, triPos2;
	triPos1 = { camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f) };

	triPos2 = { camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f) };

	// 2点を使って当たった場所をセットする場所とする
	if (!hitCheck(&this->WorldPos, triPos1, OutPos1, OutPos2))
	{
		hitCheck(&this->WorldPos, triPos2, OutPos1, OutPos2);
	}
}

//=============================================================================
// インクで通れる道を作る
//=============================================================================
void Cursor::PaintPath()
{
	float Distance = 0.0f;
	D3DXMATRIX WorldMtx, TransMtx;
	D3DXVECTOR3 DestPos_World;
	D3DXVECTOR2 DestPos_Screen;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// カーソルが始点までに移動していない
	if (AIptr->GetPaintState() == eNoAction)
	{
		DestPos_World = AIptr->GetPaintStartPos();
	}
	// ペイントしている
	else
	{
		DestPos_World = AIptr->GetPaintEndPos();
	}

	// 目標のスクリーン座標を計算する
	D3DXMatrixTranslation(&TransMtx, DestPos_World.x, DestPos_World.y, DestPos_World.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// カーソルと目標の座標を計算する
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (Distance > pow(20.0f, 2))
	{
		// カーソルが目的地に移動する
		float Angle = atan2f(Vec.y, Vec.x);
		pos.x += cosf(Angle) * CURSOR_SPEED;
		pos.y += sinf(Angle) * CURSOR_SPEED;
	}
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// ペイントが始まる
		if (AIptr->GetPaintState() == eNoAction)
		{
			AIptr->SetPaintState(ePaintStart);
		}
		// ペイント終了
		else
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
		}
	}
}

//=============================================================================
// 他のプレイヤーのペイントを削除
//=============================================================================
void Cursor::DeletePath(void)
{
	float Distance = 0.0f;
	D3DXVECTOR2 DestPos_Screen;
	D3DXMATRIX WorldMtx, TransMtx;
	GroupStruct *EnemyPaint = AIptr->GetEnemyPaint();

	// 削除が間に合わない
	if (EnemyPaint->Count <= 5 || EnemyPaint->PaintPath.empty())
	{
		AIptr->SetCursorState(eNoAction);
		AIptr->SetPaintState(ePaintEnd);
		AIptr->SetFindEnemyPaint(false);
		return;
	}

	// 目標のスクリーン座標を計算する
	D3DXMatrixTranslation(&TransMtx, EnemyPaint->PaintPath.at(0).x, EnemyPaint->PaintPath.at(0).y, EnemyPaint->PaintPath.at(0).z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// カーソルと目標の座標を計算する
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (AIptr->GetPaintState() == eNoAction)
	{
		if (Distance > pow(20.0f, 2))
		{
			// カーソルが目標に移動する
			float Angle = atan2f(Vec.y, Vec.x);
			pos.x += cosf(Angle) * CURSOR_SPEED;
			pos.y += sinf(Angle) * CURSOR_SPEED;
		}
		else
		{
			// ペイントが始まる
			AIptr->SetPaintState(ePaintStart);
		}
	}
	// ペイントしている
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// Vectorの一番目の座標を消す
		EnemyPaint->PaintPath.erase(EnemyPaint->PaintPath.begin());

		// ペイント終了
		if (EnemyPaint->PaintPath.empty())
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
			AIptr->SetFindEnemyPaint(false);
			EnemyPaint->Count = 0;
		}
	}
}

//=============================================================================
// オブジェクトチップをペイントする
//=============================================================================
void Cursor::PaintObjChip(void)
{
	float Distance = 0.0f;
	D3DXMATRIX WorldMtx, TransMtx;
	D3DXVECTOR3 DestPos_World;
	D3DXVECTOR2 DestPos_Screen;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// カーソルが始点までに移動していない
	if (AIptr->GetPaintState() == eNoAction)
	{
		DestPos_World = AIptr->GetPaintStartPos();
	}
	// ペイントしている
	else
	{
		DestPos_World = AIptr->GetPaintEndPos();
	}

	// 目標のスクリーン座標を計算する
	D3DXMatrixTranslation(&TransMtx, DestPos_World.x, DestPos_World.y, DestPos_World.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// カーソルと目標の座標を計算する
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (Distance > pow(20.0f, 2))
	{
		// カーソルが目標に移動する
		float Angle = atan2f(Vec.y, Vec.x);
		pos.x += cosf(Angle) * CURSOR_SPEED;
		pos.y += sinf(Angle) * CURSOR_SPEED;
	}
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// ペイントが始まる
		if (AIptr->GetPaintState() == eNoAction)
		{
			AIptr->SetPaintState(ePaintStart);
		}
		// ペイント終了
		else
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
		}
	}
}

