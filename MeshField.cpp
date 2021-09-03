//=============================================================================
//
// ���b�V���n�ʂ̏��� [Meshfield.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MeshField.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHFIELD_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)	// �\���ʒu
#define MESHFIELD_ROT			D3DXVECTOR3(0.0f, 0.0f, 0.0f)	// ��]
#define MESHFIELD_NUM_BLOCK_X	(10)							// �u���b�N��
#define MESHFIELD_NUM_BLOCK_Z	(10)
#define MESHFIELD_SIZE			D3DXVECTOR3(80.0f, 0.0f, 80.0f)	// �T�C�Y

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MeshField::MeshField()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ResourceManager::Instance()->GetTexture("MeshField", &D3DTexture);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pos = MESHFIELD_POS;
	rot = MESHFIELD_ROT;

	// �u���b�N���̐ݒ�
	NumBlockX = MESHFIELD_NUM_BLOCK_X;
	NumBlockZ = MESHFIELD_NUM_BLOCK_Z;

	// ���_���̐ݒ�
	NumVertex = (MESHFIELD_NUM_BLOCK_X + 1) * (MESHFIELD_NUM_BLOCK_Z + 1);

	// �C���f�b�N�X���̐ݒ�
	NumVertexIndex = (MESHFIELD_NUM_BLOCK_X + 1) * 2 * MESHFIELD_NUM_BLOCK_Z + (MESHFIELD_NUM_BLOCK_Z - 1) * 2;

	// �|���S�����̐ݒ�
	NumPolygon = MESHFIELD_NUM_BLOCK_X * MESHFIELD_NUM_BLOCK_Z * 2 + (MESHFIELD_NUM_BLOCK_Z - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	BlockSizeX = MESHFIELD_SIZE.x;
	BlockSizeZ = MESHFIELD_SIZE.z;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(Vertex3D) * NumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * NumVertexIndex,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DIdxBuff,						// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		Vertex3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (NumBlockZ + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (NumBlockX + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.x = -(NumBlockX / 2.0f) * BlockSizeX + nCntVtxX * BlockSizeX;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].vtx.z = (NumBlockZ / 2.0f) * BlockSizeZ - nCntVtxZ * BlockSizeZ;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (NumBlockX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < NumBlockZ; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockX + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (NumBlockX + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockX + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockX + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (NumBlockZ - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockX + 1) + NumBlockX;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		D3DIdxBuff->Unlock();
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
MeshField::~MeshField()
{

	SAFE_RELEASE(D3DVtxBuff);
	SAFE_RELEASE(D3DIdxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void MeshField::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void MeshField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX WorldMtx, RotMtx, TransMtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&RotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &RotMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(D3DIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertex, 0, NumPolygon);
}