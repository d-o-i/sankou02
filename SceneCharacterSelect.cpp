//=============================================================================
//
// キャラクターセレクト画面処理 [SceneCharacterSelect.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "SceneCharacterSelect.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "CircleSceneChanger.h"
#include "Sound.h"
#include "_2dobj.h"
#include "SelectLogo.h"
#include "Cursor.h"
#include "ResourceManager.h"

bool SceneCharacterSelect::AIUse[PLAYER_MAX];					// AIの使用フラグ
int SceneCharacterSelect::SelectedCharacter[PLAYER_MAX];		// 選択したキャラクターモデルの番号

#define SelectCenter_Up (D3DXVECTOR2(635.0f,175.0f))
#define SelectCenter_Down (D3DXVECTOR2(635.0f,550.0f))
#define SelectCenter_Left (D3DXVECTOR2(440.0f,360.0f))
#define SelectCenter_Right (D3DXVECTOR2(840.0f,360.0f))
#define SelectRange (D3DXVECTOR2(150.0f,150.0f))

//=============================================================================
// コンストラクタ
//=============================================================================
SceneCharacterSelect::SceneCharacterSelect()
{
	// セレクト画面のロゴ
	selectLogo = new SelectLogo();

	ResourceManager::Instance()->GetTexture("CharacterSelect", &D3DTexture);

	MakeVertex();

	SelectFrame.reserve(4);
	SelectFrame.push_back(new CharSelectFrame(SelectCenter_Right));
	SelectFrame.push_back(new CharSelectFrame(SelectCenter_Up));
	SelectFrame.push_back(new CharSelectFrame(SelectCenter_Down));
	SelectFrame.push_back(new CharSelectFrame(SelectCenter_Left));

	// セレクト用のカーソル
	int PadCount = GetPadCount();
	if (PadCount == 0)
	{
		// キーボードプレイ、一人しかいない
		cursor.push_back(new Cursor(0));
		charSelectUI.push_back(new CharSelectUI(0, false));
		SelectOver[0] = false;
		AIUse[0] = false;
	}
	else
	{
		// ゲームパッドプレイ、接続しているパッドの数のカーソルを表示
		for (int PlayerNo = 0; PlayerNo < PadCount; PlayerNo++)
		{
			cursor.push_back(new Cursor(PlayerNo));
			charSelectUI.push_back(new CharSelectUI(PlayerNo, false));
			SelectOver[PlayerNo] = false;
			AIUse[PlayerNo] = false;
		}
	}

	// AIがランダムで決める
	for (int PlayerNo = 0; PlayerNo < PLAYER_MAX; PlayerNo++)
	{
		// 1Pは手動で決定
		if (PlayerNo == 0)
			continue;

		// 2P以降
		if (PlayerNo >= PadCount)
		{
			SelectedCharacter[PlayerNo] = rand() % (3 + 1);
			charSelectUI.push_back(new CharSelectUI(PlayerNo, true));
			charSelectUI.at(PlayerNo)->SetCharTexture(SelectedCharacter[PlayerNo]);
			SelectOver[PlayerNo] = true;
			AIUse[PlayerNo] = true;
		}
	}

	/*****************************************************************************/
	// シーンチェンジの終了
	CircleSceneChanger::Instance()->SetChanger(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
SceneCharacterSelect::~SceneCharacterSelect()
{
	SAFE_DELETE(selectLogo);

	for (auto &Object : cursor)
	{
		SAFE_DELETE(Object);
	}
	cursor.clear();
	ReleaseVector(cursor);

	for (auto &Object : SelectFrame)
	{
		SAFE_DELETE(Object);
	}
	SelectFrame.clear();
	ReleaseVector(SelectFrame);

	for (auto &Object : charSelectUI)
	{
		SAFE_DELETE(Object);
	}
	charSelectUI.clear();
	ReleaseVector(charSelectUI);
}

//=============================================================================
// 更新
//=============================================================================
void SceneCharacterSelect::Update(int SceneID)
{
	// シーンチェンジ
	if (SelectOver[0] && SelectOver[1] &&
		SelectOver[2] && SelectOver[3])
	{
		CircleSceneChanger::Instance()->SetChanger(true, []()
		{
			SetScene(nSceneGame);
		});
		return;
	}

	// フラグ初期化
	for (auto &Object : SelectFrame)
	{
		Object->SetUse(false);
	}

	// キャラクター選択
	for (int playerNo = 0; playerNo < PLAYER_MAX; playerNo++)
	{
		if (!SelectOver[playerNo])
		{
			if (CheckSelect(playerNo))
			{
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(playerNo, BUTTON_C))
				{
					PlaySound(SE_CHOICE);
					charSelectUI.at(playerNo)->SetCharTexture(SelectedCharacter[playerNo]);
					SelectOver[playerNo] = true;
				}
			}
		}
	}

	// カーソルの更新
	for (auto & Object : cursor)
	{
		Object->Update();
	}

	// セレクトフレーム更新
	for (auto &Object : SelectFrame)
	{
		Object->Update();
	}

	// ロゴ更新
	selectLogo->Update();
}

//=============================================================================
// 描画
//=============================================================================
void SceneCharacterSelect::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);
	Device->SetTexture(0, D3DTexture);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

	// セレクトロゴの描画
	selectLogo->Draw();

	// セレクトフレームの描画
	for (auto &Object : SelectFrame)
	{
		Object->Draw();
	}

	// セレクトUIの描画
	for (auto &Object : charSelectUI)
	{
		Object->Draw();
	}

	// カーソルの描画
	for (auto & Object : cursor)
	{
		Object->Draw();
	}
}


//=============================================================================
// 頂点の作成
//=============================================================================
void SceneCharacterSelect::MakeVertex(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// セレクト座標をチェックする
//=============================================================================
bool SceneCharacterSelect::CheckSelect(int PlayerNo)
{
	D3DXVECTOR3 CursorPos = cursor.at(PlayerNo)->GetPos();

	if (CursorPos.x > SelectCenter_Right.x - SelectRange.x / 2 &&
		CursorPos.x < SelectCenter_Right.x + SelectRange.x / 2 &&
		CursorPos.y > SelectCenter_Right.y - SelectRange.y / 2 &&
		CursorPos.y < SelectCenter_Right.y + SelectRange.y / 2)
	{
		SelectedCharacter[PlayerNo] = 0;
		SelectFrame.at(0)->SetUse(true);
		return true;
	}

	if (CursorPos.x > SelectCenter_Up.x - SelectRange.x / 2 &&
		CursorPos.x < SelectCenter_Up.x + SelectRange.x / 2 &&
		CursorPos.y > SelectCenter_Up.y - SelectRange.y / 2 &&
		CursorPos.y < SelectCenter_Up.y + SelectRange.y / 2)
	{
		SelectedCharacter[PlayerNo] = 1;
		SelectFrame.at(1)->SetUse(true);
		return true;
	}

	if (CursorPos.x > SelectCenter_Down.x - SelectRange.x / 2 &&
		CursorPos.x < SelectCenter_Down.x + SelectRange.x / 2 &&
		CursorPos.y > SelectCenter_Down.y - SelectRange.y / 2 &&
		CursorPos.y < SelectCenter_Down.y + SelectRange.y / 2)
	{
		SelectedCharacter[PlayerNo] = 2;
		SelectFrame.at(2)->SetUse(true);
		return true;
	}

	if (CursorPos.x > SelectCenter_Left.x - SelectRange.x / 2 &&
		CursorPos.x < SelectCenter_Left.x + SelectRange.x / 2 &&
		CursorPos.y > SelectCenter_Left.y - SelectRange.y / 2 &&
		CursorPos.y < SelectCenter_Left.y + SelectRange.y / 2)
	{
		SelectedCharacter[PlayerNo] = 3;
		SelectFrame.at(3)->SetUse(true);
		return true;
	}

	return false;
}

//=============================================================================
// キャラクターセレクト番号のゲッター
//=============================================================================
int SceneCharacterSelect::GetSelectCharacter(int PlayerNo)
{
	return SelectedCharacter[PlayerNo];
}

//=============================================================================
// ゲッター
//=============================================================================
bool SceneCharacterSelect::GetAIUse(int PlayerNo)
{
	return AIUse[PlayerNo];
}