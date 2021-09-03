//=============================================================================
//
// �L�����N�^�[�Z���N�g�t���[�� [CharSelectFrame.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "CharSelectFrame.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ImageSize		D3DXVECTOR3(150.0f, 150.0f, 0.0f)		// �e�N�X�`���T�C�Y

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CharSelectFrame::CharSelectFrame(D3DXVECTOR2 Pos)
{
	ResourceManager::Instance()->GetTexture("CharSelectFrame", &D3DTexture);

	this->use = false;
	this->pos = (D3DXVECTOR3)Pos;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CharSelectFrame::~CharSelectFrame()
{
	// ���\�[�X�̊J���̓��\�[�X�}�l�[�W���ɔC����̂ŁA�Q�Ƃ���߂邾��
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void CharSelectFrame::Update()
{
	if (use == true)
	{
		if (AlphaPlus)
		{
			Alpha += 5;
			if (Alpha >= 192)
			{
				Alpha = 192;
				AlphaPlus = false;
			}
		}
		else
		{
			Alpha -= 5;
			if (Alpha <= 0)
			{
				Alpha = 0;
				AlphaPlus = true;
			}
		}

		SetAlpha(Alpha);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CharSelectFrame::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (use == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT CharSelectFrame::MakeVertex(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - ImageSize.x / 2, pos.y - ImageSize.y / 2, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + ImageSize.x / 2, pos.y - ImageSize.y / 2, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - ImageSize.x / 2, pos.y + ImageSize.y / 2, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + ImageSize.x / 2, pos.y + ImageSize.y / 2, 0.0f);

	// rhw�̐ݒ�
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 192);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �A���t�@�l�̐ݒ�
//=============================================================================
void CharSelectFrame::SetAlpha(int Alpha)
{
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, Alpha);
}
