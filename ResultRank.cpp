//=============================================================================
//
// �v���C���[���� [ResultRank.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultRank.h"
#include "Player.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULTRANK_POS	D3DXVECTOR3(130.0f, 180.0f, 0.0f)	// 1�ʂ��
#define RESULTRANK_SIZE	D3DXVECTOR3(170.0f, 85.0f, 0.0f)
#define RESULTRANK_INTERVAL	(80.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ResultRank::ResultRank(int rank, int owner)
{
	ResourceManager::Instance()->GetTexture("ResultRank", &D3DTexture);

	this->use = true;
	this->rank = rank;
	this->owner = owner;
	pos = RESULTRANK_POS + D3DXVECTOR3(0.0f, RESULTRANK_INTERVAL * rank, 0.0f);
	size = RESULTRANK_SIZE;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ResultRank::~ResultRank()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void ResultRank::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void ResultRank::Draw()
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
// ���_���̍쐬
//=============================================================================
HRESULT ResultRank::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

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
	int x = owner % RESULTRANK_DIVIDE_X;
	int y = owner / RESULTRANK_DIVIDE_X;
	float sizeX = 1.0f / RESULTRANK_DIVIDE_X;
	float sizeY = 1.0f / RESULTRANK_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	return S_OK;
}