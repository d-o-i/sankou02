//=============================================================================
//
// プレイヤーUI処理 [PlayerUI.h]
// Author :  
//
//=============================================================================
#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_

#include "Pop.h"
#include "MiniPlayer.h"
#include "Faceframe.h"
#include "Face.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class PlayerUI
{
private:
	Pop			*PopUp;
	MiniPlayer	*miniPlayer;
	FaceFrame	*faceFrame;
	Face		*face;

public:
	PlayerUI(int PlayerNo);
	~PlayerUI();

	void Update(D3DXVECTOR3 PlayerPos);
	void Draw(bool OnCamera, bool blind);
	void SetPlayerDeadTexture(void) { this->faceFrame->SetPlayerDeadTexture(); };
};

#endif

