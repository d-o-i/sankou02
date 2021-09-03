//=============================================================================
//
// �t�B�j�b�V��UI [Finish.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Finish.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FINISH_SIZE		D3DXVECTOR3(1024.0f, 512.0f, 0.0f)
#define FINISH_MOVE_SPEED	(100.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Finish::Finish()
{
	ResourceManager::Instance()->GetTexture("Finish", &D3DTexture);

	use = true;
	pos = D3DXVECTOR3(SCREEN_WIDTH + FINISH_SIZE.x / 2, SCREEN_CENTER_Y, 0.0f);
	PatternAnim = 1;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Finish::~Finish()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Finish::Update()
{
	if(use)
	{
		// �E����^�񒆂ɏo�Ă���
		if (pos.x > SCREEN_CENTER_X)
		{
			pos.x -= FINISH_MOVE_SPEED;
		}
		else
		{			
			pos.x = SCREEN_CENTER_X;
		}

		SetVertex();
	}
}

//=============================================================================
// �`��
//=============================================================================
void Finish::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Finish::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex();

	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̍쐬
//=============================================================================
void Finish::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - FINISH_SIZE.x / 2, pos.y - FINISH_SIZE.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + FINISH_SIZE.x / 2, pos.y - FINISH_SIZE.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - FINISH_SIZE.x / 2, pos.y + FINISH_SIZE.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + FINISH_SIZE.x / 2, pos.y + FINISH_SIZE.y / 2, pos.z);
}