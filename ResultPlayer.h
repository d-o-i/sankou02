//=============================================================================
//
// ���U���g��ʂł̃v���C���[ [ResultPlayer.cpp]
// Author :  
//
//=============================================================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "D3DXAnimation.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ResultPlayer :
	public D3DXANIMATION
{
private:
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet();

public:
	ResultPlayer(int rank, int owner);
	~ResultPlayer();

	void Update()override;
	void Draw()override;
};

#endif
