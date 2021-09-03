//=============================================================================
//
// �^�C�g����ʏ��� [Title.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Title.h"
#include "CursorObj.h"
#include "Sound.h"
#include "Input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneTitle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_SIZE			D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.75f, 0.0f)	// �e�N�X�`���T�C�Y
#define LOGO_POS			D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.75f, 0.0f)		// �e�N�X�`�����W

#define MENU_SIZE			D3DXVECTOR3(702.0f * 0.45f, 515.0f * 0.45f, 0.0f)				// �e�N�X�`���T�C�Y
#define MENU_POS			D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.65f, 0.0f)		// �e�N�X�`�����W
//=============================================================================
// �R���X�g���N�^
//=============================================================================
TITLE::TITLE(int num)
{
	switch (num)
	{
	case TitleLogo:
		ResourceManager::Instance()->GetTexture("TitleLogo", &D3DTexture);
		size = LOGO_SIZE;
		pos = LOGO_POS;
		break;
	case TitleMenu:
		ResourceManager::Instance()->GetTexture("TitleMenu", &D3DTexture);
		size = MENU_SIZE;
		pos = MENU_POS;
		break;

	default:
		break;
	}

	use = true;
	PatternAnim = 1;

	// ���_���̍쐬
	MakeVertex();

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
TITLE::~TITLE()
{
	// ���\�[�X�̊J���̓��\�[�X�}�l�[�W���ɔC���邽�߁A�Q�Ƃ���߂邾��
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void  TITLE::Update()
{

	if (use == true)
	{

		// �e�N�X�`�����W���Z�b�g
		SetTexture(PatternAnim);

		SetVertex();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TITLE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	if (use == true)
	{
		// �e�N�X�`���̐ݒ�i�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s���j
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����\��Ȃ����Ƃ��w�肷��pDevice->SetTexture(0,NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT TITLE::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
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
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void TITLE::SetTexture(int cntPattern)
{
	int x = cntPattern;
	int y = cntPattern;
	float sizeX = 1.0f;
	float sizeY = 1.0f;


	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void TITLE::SetVertex(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// ���_���W�̈ړ�
//=============================================================================
void TITLE::SetVertexMove(D3DXVECTOR3 pos)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);
}
