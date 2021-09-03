//=============================================================================
//
// �|�b�v�A�b�v�\������ [Pop.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Pop.h"
#include "Camera.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POP_WIDTH		(64.0f)							// ���a����
#define	POP_HEIGHT		(32.0f)							// ���a��
#define POP_POS			D3DXVECTOR3(0.0f, 90.0f, -1.0f)	// �\���ꏊ
#define POP_DIVIDE_X	(4)
#define POP_DIVIDE_Y	(1)
#define POP_ANIM_DIVIDE	(POP_DIVIDE_X * POP_DIVIDE_Y)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Pop::Pop(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("Pop", &D3DTexture);

	this->PlayerNo = PlayerNo;
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	width = POP_WIDTH;
	height = POP_HEIGHT;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Pop::~Pop()
{
	// �e�N�X�`���̎Q�Ƃ���߂�
	D3DTexture = NULL;

	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(D3DVtxBuff);
}

//=============================================================================
// �X�V����
//=============================================================================
void Pop::Update(D3DXVECTOR3 PlayerPos)
{
	pos = PlayerPos + POP_POS;
}

//=============================================================================
// �`�揈��
//=============================================================================
void Pop::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX WorldMtx, TransMtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &WorldMtx);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTexture);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Pop::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, POP_HEIGHT / 2.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, POP_HEIGHT / 2.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, -POP_HEIGHT / 2.0f, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, -POP_HEIGHT / 2.0f, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	int x = this->PlayerNo;
	float sizeX = 1.0f / (float)POP_DIVIDE_X;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

	return S_OK;
}