//=============================================================================
//
// �~�j�v���C���[���� [MiniPlayer.h]
// Author :  
//
//=============================================================================
#ifndef _MINIPLAYER_H_
#define _MINIPLAYER_H_

#include "_2dobj.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class MiniPlayer : public _2dobj
{
private:
	HRESULT MakeVertex(int PlayerNo);	// ���_���̍쐬
	void SetTexture(int PlayerNo);		// �e�N�X�`�����W�̐ݒ�
	void SetVertex(void);				// ���_���W�̐ݒ�

public:
	MiniPlayer(int PlayerNo);
	~MiniPlayer();

	void Update(D3DXVECTOR3 PlayerPos);
	void Draw();
};

#endif

