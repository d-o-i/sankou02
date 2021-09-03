//=============================================================================
//
// プレイヤーUI処理 [PlayerUI.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "PlayerUI.h"
#include "SceneCharacterSelect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
PlayerUI::PlayerUI(int PlayerNo)
{
	this->PopUp = new Pop(PlayerNo);
	this->miniPlayer = new MiniPlayer(PlayerNo);
	this->face = new Face(PlayerNo, SceneCharacterSelect::GetSelectCharacter(PlayerNo));
	this->faceFrame = new FaceFrame(PlayerNo);
}

//=============================================================================
// デストラクタ
//=============================================================================
PlayerUI::~PlayerUI()
{
	SAFE_DELETE(this->PopUp);
	SAFE_DELETE(this->miniPlayer);
	SAFE_DELETE(this->faceFrame);
	SAFE_DELETE(this->face);
}

//=============================================================================
// 更新
//=============================================================================
void PlayerUI::Update(D3DXVECTOR3 PlayerPos)
{
	PopUp->Update(PlayerPos);
	miniPlayer->Update(PlayerPos);
	face->Update();
	faceFrame->Update();
}

//=============================================================================
// 描画
//=============================================================================
void PlayerUI::Draw(bool OnCamera, bool blind)
{
	if (OnCamera)
	{
		if (!blind)
		{
			PopUp->Draw();
		}
		miniPlayer->Draw();
	}

	face->Draw();
	faceFrame->Draw();
}
