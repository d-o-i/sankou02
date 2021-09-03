//=============================================================================
//
// �~�j�v���C���[���� [MiniPlayer.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MiniPlayer.h"
#include "Map.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MiniPlayer_Divide_X		(4)									// ������
#define MiniPlayer_Size_X		(15)
#define MiniPlayer_Size_Y		(30)
#define MiniPlayer_Pos_X		(10)
#define MiniPlayer_Pos_Y		(80)
#define Distance_Screen			(1200.0f)
#define Distance_World			(GOAL_POS.x - START_POS.x)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MiniPlayer::MiniPlayer(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("MiniPlayer", &D3DTexture);

	pos = D3DXVECTOR3(MiniPlayer_Pos_X, MiniPlayer_Pos_Y, 0.0f);

	// ���_���̍쐬
	MakeVertex(PlayerNo);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
MiniPlayer::~MiniPlayer()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void MiniPlayer::Update(D3DXVECTOR3 PlayerPos)
{
	pos.x = MiniPlayer_Pos_X + Distance_Screen * (PlayerPos.x / Distance_World);

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// �`��
//=============================================================================
void MiniPlayer::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);

	Device->SetTexture(0, D3DTexture);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}
//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MiniPlayer::MakeVertex(int PlayerNo)
{
	// ���_���W�̐ݒ�
	SetVertex();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTexture(PlayerNo);

	return S_OK;

}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void MiniPlayer::SetTexture(int PlayerNo)
{
	float SizeX = 1.0f / MiniPlayer_Divide_X;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)PlayerNo* SizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)PlayerNo* SizeX + SizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)PlayerNo* SizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)PlayerNo* SizeX + SizeX, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void MiniPlayer::SetVertex(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + MiniPlayer_Size_X, pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + MiniPlayer_Size_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + MiniPlayer_Size_X, pos.y + MiniPlayer_Size_Y, 0.0f);
}