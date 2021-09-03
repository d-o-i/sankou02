//=============================================================================
//
// �p�[�e�B�N�� [Particle.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Particle.h"
#include "Camera.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_FILE	("data/TEXTURE/Effect.jpg")

//*****************************************************************************
// �����o�ϐ��̏�����
//*****************************************************************************
LPDIRECT3DTEXTURE9 Particle::D3DTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Particle::Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scl, int time)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (D3DTexture == NULL)
	{
		D3DXCreateTextureFromFile(Device, EFFECT_FILE, &D3DTexture);
	}

	// �������W�A�F�A�e�N�X�`���������_���Ŏw�肷��
	this->pos = pos;
	this->scl = scl;
	this->move = move;
	this->col = col;
	this->time = time;
	this->decAlpha = (float)this->col.a / (float)this->time;
	this->use = true;
	this->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Particle::~Particle()
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(this->D3DVtxBuff);
}

//=============================================================================
// �X�V
//=============================================================================
void Particle::Update()
{
	if (!use)
		return;

	this->pos += this->move;

	// �����x�̑���
	this->col.a -= this->decAlpha;
	if (this->col.a <= 0)
	{
		this->use = false;
	}

	SetTexture();
	SetVertex();
	SetColor();
}

//=============================================================================
// �`��
//=============================================================================
void Particle::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

	// ���C�e�B���O�𖳌��ɂ���
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

#if 0
	// ���Z���� �����_�����O�X�e�[�g�̕ύX�������ۂ��Ȃ�i���Z�����͔����ۂ��Ȃ�i255�ɋ߂Â��Ă����Ɓj�j
	//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�e�X�g
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
#endif
	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (use)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&WorldMtx);

		// �r���[�}�g���b�N�X���擾
		ViewMtx = cameraWk->mtxView;

		// �|���S���𐳖ʂɌ�����
		WorldMtx._11 = ViewMtx._11;
		WorldMtx._12 = ViewMtx._21;
		WorldMtx._13 = ViewMtx._31;
		WorldMtx._21 = ViewMtx._12;
		WorldMtx._22 = ViewMtx._22;
		WorldMtx._23 = ViewMtx._32;
		WorldMtx._31 = ViewMtx._13;
		WorldMtx._32 = ViewMtx._23;
		WorldMtx._33 = ViewMtx._33;

		// �t�s������Ƃ߂�
		D3DXMatrixInverse(&WorldMtx, NULL, &ViewMtx);
		WorldMtx._41 = 0.0f;
		WorldMtx._42 = 0.0f;
		WorldMtx._43 = 0.0f;

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
		D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

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
		Device->SetTexture(0, Particle::D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

#if 1
	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif

	// ���C���e�B���O��L���ɂ���
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Particle::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-scl.x / 2, scl.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(scl.x / 2, scl.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-scl.x / 2, -scl.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(scl.x / 2, -scl.y / 2, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

	return S_OK;

}

//=============================================================================
// ���_���W�̃Z�b�g
//=============================================================================
void Particle::SetVertex()
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-scl.x / 2, scl.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(scl.x / 2, scl.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-scl.x / 2, -scl.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(scl.x / 2, -scl.y / 2, 0.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

}

//=============================================================================
// ���ˌ��̃Z�b�g
//=============================================================================
void Particle::SetColor()
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

}

//=============================================================================
// �e�N�X�`�����W�̃Z�b�g
//=============================================================================
void Particle::SetTexture()
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

}