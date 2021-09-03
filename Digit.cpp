//=============================================================================
//
// �� [Digit.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Digit.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIGIT_SIZE		D3DXVECTOR3(30.0f, 50.0f, 0.0f)
#define DIGIT_SPACE		(40.0f)	// �\���Ԋu�i���j
//#define DIGIT_POS		D3DXVECTOR3(SCREEN_CENTER_X + ((DIGIT_SIZE.x / 2) * DIGIT_MAX), 135.0f, 0.0f)	// �\���ꏊ�i1�ԉE�A1���ڂ���Ƃ���j
#define DIGIT_POS		D3DXVECTOR3((SCREEN_CENTER_X + 120.0f), 135.0f, 0.0f)	// �\���ꏊ�i1�ԉE�A1���ڂ���Ƃ���j

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Digit::Digit(int digit)
{
	ResourceManager::Instance()->GetTexture("Digit", &D3DTexture);
	use = true;
	time = 0;
	pos = DIGIT_POS;
	pos.x -= digit * DIGIT_SPACE;
	// �~���b�ƕb�A�b�ƕ��̊Ԃ�':'�̃X�y�[�X������
	if (digit >= 2)
	{
		pos.x -= 20.0f;
	}
	if (digit >= 4)
	{
		pos.x -= 20.0f;
	}
	size = DIGIT_SIZE;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Digit::~Digit()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Digit::Update()
{
}

//=============================================================================
// �X�V
//=============================================================================
void Digit::Update(DWORD time)
{
	this->time = time;
	SetTexture();
}

//=============================================================================
// �`��
//=============================================================================
void Digit::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// Z�e�X�g
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		Device->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Digit::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);

	// rhw�̐ݒ�
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
	SetTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̃Z�b�g
//=============================================================================
void Digit::SetTexture()
{
	int x = time % DIGIT_DIVIDE_X;
	int y = time / DIGIT_DIVIDE_X;
	float sizeX = 1.0f / DIGIT_DIVIDE_X;
	float sizeY = 1.0f / DIGIT_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}