//=============================================================================
//
// �� [Sky.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Sky.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SKY_NUM_X		(13)						// ���ɕ��ׂ鐔
#define SKY_NUM_Y		(3)							// �c�ɕ��ׂ鐔

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Sky::Sky()
{
	ResourceManager::Instance()->GetTexture("Sky", &D3DTexture);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 30.0f);

	MakeVertex();

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Sky::~Sky()
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(this->D3DVtxBuff);

	// �e�N�X�`���̎Q�Ƃ��I��
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Sky::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void Sky::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	D3DXVECTOR3 oldpos = pos;

	// ���e�X�g��L����
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int cntY = 0; cntY < SKY_NUM_Y; cntY++)
	{
		for (int cntX = 0; cntX < SKY_NUM_X; cntX++)
		{
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

			// �e�N�X�`���̃Z�b�g
			pDevice->SetTexture(0, D3DTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			pos.x += SCREEN_WIDTH;
		}

		pos.x -= SCREEN_WIDTH * SKY_NUM_X;
		pos.y -= SCREEN_HEIGHT;
	}
	pos = oldpos;

	// ���e�X�g�𖳌���
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_���W�̍쐬
//=============================================================================
HRESULT Sky::MakeVertex()
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
		pVtx[0].vtx = D3DXVECTOR3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}


	return S_OK;
}