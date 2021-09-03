//=============================================================================
//
// �`�b�v���� [Chip.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Chip.h"
#include "Camera.h"
#include "Map.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHIP_DIVIDE_X	(8)
#define CHIP_DIVIDE_Y	(16)

#define OBJ_DIVIDE_X	(7)
#define OBJ_DIVIDE_Y	(1)

#define REVERSE_TIME	(300)	// ���]����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Chip::Chip(int x, int y, int _texnum, int ChipType)
{
	this->ChipType = ChipType;

	switch (ChipType)
	{
		// �}�b�v�`�b�v
	case eMapChip:
		ResourceManager::Instance()->GetTexture("MapChip", &D3DTexture);
		break;
		// �I�u�W�F�N�g�`�b�v
	case eObjectChip:
		ResourceManager::Instance()->GetTexture("ObjectChip", &D3DTexture);
		break;
	}

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = D3DXVECTOR3(x * CHIP_SIZE, -(y * CHIP_SIZE), 0.0f);
	use = true;
	texnum = _texnum;
	cnt = 0;
	reverse = false;
	mapX = x;
	mapY = y;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Chip::~Chip()
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(this->D3DVtxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Chip::Update()
{
	CheckOnCamera();

	// ���]���̏���
	if (use && reverse)
	{
		cnt++;

		if (cnt == REVERSE_TIME)
		{
			cnt = 0;
			reverse = false;
			ReverseTexture();
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void Chip::Draw()
{
	if (use)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Chip::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		Vertex3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-CHIP_SIZE / 2, CHIP_SIZE / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(CHIP_SIZE / 2, CHIP_SIZE / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-CHIP_SIZE / 2, -CHIP_SIZE / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(CHIP_SIZE / 2, -CHIP_SIZE / 2, 0.0f);

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

		// �e�N�X�`�����W�̐ݒ�
		if (texnum < 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
		else
		{
			if (ChipType == eMapChip)
			{
				int x = texnum % CHIP_DIVIDE_X;
				int y = texnum / CHIP_DIVIDE_X;
				float sizeX = 1.0f / CHIP_DIVIDE_X;
				float sizeY = 1.0f / CHIP_DIVIDE_Y;

				pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
				pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
				pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
				pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
			}
			else if (ChipType == eObjectChip)
			{
				float sizeX = 1.0f / OBJ_DIVIDE_X;

				pVtx[0].tex = D3DXVECTOR2((float)texnum * sizeX, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((float)texnum * sizeX + sizeX, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((float)texnum * sizeX, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((float)texnum * sizeX + sizeX, 1.0f);
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̃Z�b�g
//=============================================================================
void Chip::SetTexture()
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		Vertex3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W�̐ݒ�
		if (texnum < 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
		else
		{
			if (ChipType == eMapChip)
			{
				int x = texnum % CHIP_DIVIDE_X;
				int y = texnum / CHIP_DIVIDE_X;
				float sizeX = 1.0f / CHIP_DIVIDE_X;
				float sizeY = 1.0f / CHIP_DIVIDE_Y;

				pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
				pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
				pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
				pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
			}
			else if (ChipType == eObjectChip)
			{
				float sizeX = 1.0f / OBJ_DIVIDE_X;

				pVtx[0].tex = D3DXVECTOR2((float)texnum * sizeX, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((float)texnum * sizeX + sizeX, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((float)texnum * sizeX, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((float)texnum * sizeX + sizeX, 1.0f);
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}

}

//=============================================================================
// �J����������
//=============================================================================
void Chip::CheckOnCamera()
{
	CAMERA *camera = GetCamera();

	// �`��͈͔���
	// �c
	if ((pos.x > camera->at.x - DRAW_RANGE.x) && (pos.x < camera->at.x + DRAW_RANGE.x))
	{
		// ��
		if ((pos.y > camera->at.y - DRAW_RANGE.y) && (pos.y < camera->at.y + DRAW_RANGE.y))
		{
			use = true;
		}
		else
		{
			use = false;
		}
	}
	else
	{
		use = false;
	}
}

//=============================================================================
// �e�N�X�`���𔽓]������
//=============================================================================
void Chip::ReverseTexture()
{
	switch (texnum)
	{
	case eObjSpdup:
		texnum = eObjSpddown;
		break;
	case eObjSpddown:
		texnum = eObjSpdup;
		break;
	case eObjDrain:
		texnum = eObjHeal;
		break;
	case eObjHeal:
		texnum = eObjDrain;
		break;
	default:
		break;
	}

	// �I�u�W�F�N�g�e�[�u���̒��g���ύX����
	Map::SetObjTbl(mapX, mapY, texnum);
	SetTexture();
}