//=============================================================================
//
// �L�����N�^�[�̊�\�� [Face.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Face.h"
#include "CursorObj.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FACE_SIZE		D3DXVECTOR3(75.0f, 75.0f, 0.0f)		// �e�N�X�`���T�C�Y
//#define FACE_SIZE		D3DXVECTOR3(50.0f, 50.0f, 0.0f)		// �e�N�X�`���T�C�Y
#define FACE_POS		D3DXVECTOR3(17.0f, 19.0f, 0.0f)
//#define FACE_POS		D3DXVECTOR3(31.0f, 30.5f, 0.0f)
#define FACE_SPACE		(318.0f)							// �\���Ԋu

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Face::Face(int playerNo, int charNo)
{
	ResourceManager::Instance()->GetTexture("SelectCursor", &D3DTexture);

	this->playerNo = playerNo;
	this->charNo = charNo;

	use = true;
	pos = FACE_POS;
	pos.x += this->playerNo * FACE_SPACE;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Face::~Face()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void Face::Update()
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void Face::Draw()
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
// ���_�̍쐬
//=============================================================================
HRESULT Face::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex();

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

	SetTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void Face::SetTexture()
{
	int x = this->charNo;
	float sizeX = 1.0f / (float)CURSOROBJ_DIVIDE_X;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.51f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.51f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void Face::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + FACE_SIZE.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + FACE_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + FACE_SIZE.x, pos.y + FACE_SIZE.y, pos.z);
}