//=============================================================================
//
// �\����[Podium.h]
// Author : 
//
//=============================================================================
#ifndef _PODIUM_H_
#define _PODIUM_H_

#include "D3DXAnimation.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Podium :
	public D3DXANIMATION
{
private:
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	void CreateAnimSet();

public:
	Podium();
	~Podium();

	void Update()override;
	void Draw()override;
};

#endif
