//=============================================================================
//
// �󏈗� [SkyBox.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "SkyBox.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SKYBOX_DIVIDE_X			(4)									// �e�N�X�`��������
#define SKYBOX_DIVIDE_Y			(3)
#define SURFACE_NUM				(6)									// �ʂ̐�
#define SKYBOX_LENGTH			(3000.0f)							// �傫��
#define SKYBOX(Alpha)			D3DCOLOR_RGBA(135, 206, 235, Alpha)	// ��̐F

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
SkyBox::SkyBox()
{
	ResourceManager::Instance()->GetTexture("SkyBox", &D3DTexture);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = D3DXVECTOR3(0.0f, -500.0f, 0.0f);;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// ���_���̍쐬
	MakeVertex();

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
SkyBox::~SkyBox()
{
	SAFE_RELEASE(D3DVtxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void SkyBox::Update()
{
	// ������Ƃ���]
	rot.y += 0.0001f;

}

//=============================================================================
// �`�揈��
//=============================================================================
void SkyBox::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX WorldMtx, SclMtx, RotMtx, TransMtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&RotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &RotMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTexture);

	// �|���S���̕`��
	for (int i = 0; i < SURFACE_NUM; i++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT SkyBox::MakeVertex()
{
	int i = 0;
	int x = 0, y = 0;
	float sizeX = 1.0f / SKYBOX_DIVIDE_X;
	float sizeY = 1.0f / SKYBOX_DIVIDE_Y;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX * SURFACE_NUM,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�

	// �O
	pVtx[0].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[1].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[2].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[3].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	// ���
	pVtx[4].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[5].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[6].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[7].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	// ��
	pVtx[8].vtx = pVtx[5].vtx;
	pVtx[9].vtx = pVtx[4].vtx;
	pVtx[10].vtx = pVtx[0].vtx;
	pVtx[11].vtx = pVtx[1].vtx;
	// �E
	pVtx[12].vtx = pVtx[1].vtx;
	pVtx[13].vtx = pVtx[4].vtx;
	pVtx[14].vtx = pVtx[3].vtx;
	pVtx[15].vtx = pVtx[6].vtx;
	// ��
	pVtx[16].vtx = pVtx[2].vtx;
	pVtx[17].vtx = pVtx[3].vtx;
	pVtx[18].vtx = pVtx[7].vtx;
	pVtx[19].vtx = pVtx[6].vtx;
	// ��
	pVtx[20].vtx = pVtx[5].vtx;
	pVtx[21].vtx = pVtx[0].vtx;
	pVtx[22].vtx = pVtx[7].vtx;
	pVtx[23].vtx = pVtx[2].vtx;


	// �@���x�N�g���̐ݒ�
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// ���ˌ��̐ݒ�
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�

	// �O
	x = 1;
	y = 1;
	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// ���
	x = 3;
	y = 1;
	pVtx[4].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[5].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[6].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[7].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// ��
	x = 1;
	y = 0;
	pVtx[8].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[9].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[10].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[11].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// �E
	x = 2;
	y = 1;
	pVtx[12].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[13].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[14].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[15].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// ��
	x = 1;
	y = 2;
	pVtx[16].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[17].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[18].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[19].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// ��
	x = 0;
	y = 1;
	pVtx[20].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[21].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[22].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[23].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

	return S_OK;
}
