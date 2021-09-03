//=============================================================================
//
// �X�e�[�W���\�� [StageName.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageName.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGENAME_POS	D3DXVECTOR3(SCREEN_CENTER_X / 2, SCREEN_CENTER_Y + SCREEN_CENTER_Y / 2, 0.0f)
#define STAGENAME_SIZE	D3DXVECTOR3(SCREEN_WIDTH / 2.5f, SCREEN_HEIGHT / 2.5f, 0.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StageName::StageName()
{
	ResourceManager::Instance()->GetTexture("StageName", &D3DTexture);

	pos = STAGENAME_POS;
	size = STAGENAME_SIZE;
	PatternAnim = 1;
	use = true;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
StageName::~StageName()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void StageName::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void StageName::Draw()
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
// ���_���W�̍쐬
//=============================================================================
void StageName::MakeVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

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

}