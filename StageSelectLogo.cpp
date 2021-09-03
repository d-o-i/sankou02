//=============================================================================
//
// �X�e�[�W�Z���N�g��ʂ̃��S [StageSelectLogo.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageSelectLogo.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIALLOGO_POS	D3DXVECTOR3((float)SCREEN_CENTER_X, 100.0f, 0.0f)
#define TUTORIALLOGO_SIZE	D3DXVECTOR3(512.0f, 128.0f, 0.0f)
#define FLASH_TIME			(45)
#define DELETE_TIME			(30)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StageSelectLogo::StageSelectLogo()
{
	ResourceManager::Instance()->GetTexture("StageSelectLogo", &D3DTexture);

	pos = TUTORIALLOGO_POS;
	size = TUTORIALLOGO_SIZE;
	use = true;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
StageSelectLogo::~StageSelectLogo()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void StageSelectLogo::Update()
{
	if (!use)
		return;

	// �_�ŏ���
	cntFlash++;

	// �\�����Ɉ��t���[���o�߂ŏ���
	if (flash && cntFlash == FLASH_TIME)
	{
		flash = false;
		cntFlash = 0;
	}
	// ���Œ��Ɉ��t���[���o�߂ŕ\��
	else if (!flash && cntFlash == DELETE_TIME)
	{
		flash = true;
		cntFlash = 0;
	}

}

//=============================================================================
// �`��
//=============================================================================
void StageSelectLogo::Draw()
{
	// �g�p���Ă��Ȃ�or�_�Œ��͕\�����Ȃ�
	if (!use || !flash)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, D3DTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void StageSelectLogo::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		// rhw�̐ݒ�
		vertexWk[i].rhw = 1.0f;
		// ���ˌ��̐ݒ�
		vertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

}