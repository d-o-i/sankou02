//=============================================================================
//
// �t�B�[���h��ɐ��������A�C�e�� [FieldItem.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "FieldItem.h"
#include "Item.h"
#include "Camera.h"
#include "ResourceManager.h"

//*****************************************************************************
// �����o�̏�����
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9	FieldItem::D3DVtxBuff = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
FieldItem::FieldItem(int _texno, D3DXVECTOR3 _pos, D3DXVECTOR3 _move)
{
	ResourceManager::Instance()->GetTexture("FieldItem", &D3DTexture);

	pos = _pos;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = _move;
	width = FIELDITEM_SIZE.x;
	height = FIELDITEM_SIZE.y;
	texno = _texno;
	use = false;

	MakeVertex();

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
FieldItem::~FieldItem()
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(D3DVtxBuff);
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void FieldItem::Update()
{
	if (use)
	{
		pos += move;

		OnCamera();
	}
}

//=============================================================================
// �`��
//=============================================================================
void FieldItem::Draw()
{
	if (use)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		CAMERA *cameraWk = GetCamera();
		D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

		// ���C���e�B���O�𖳌��ɂ���
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���Z���� �����_�����O�X�e�[�g�̕ύX�������ۂ��Ȃ�i���Z�����͔����ۂ��Ȃ�i255�ɋ߂Â��Ă����Ɓj�j
		//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
		//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		// Z�e�X�g
		//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

#if 0
	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
		Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#endif
		// Z�e�X�g
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		// ���e�X�g��L����
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&WorldMtx);

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
		Device->SetTexture(0, FieldItem::D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ���C���e�B���O��L���ɂ���
		Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		// ���e�X�g�𖳌���
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
#if 0
		// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
		Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif
		// Z��r����
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT FieldItem::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(Vertex3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
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
	pVtx[0].vtx = D3DXVECTOR3(-FIELDITEM_SIZE.x / 2, FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(FIELDITEM_SIZE.x / 2, FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-FIELDITEM_SIZE.x / 2, -FIELDITEM_SIZE.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(FIELDITEM_SIZE.x / 2, -FIELDITEM_SIZE.y / 2, 0.0f);

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
	int x = texno % DIVIDE_ITEM_X;
	float sizeX = 1.0f / DIVIDE_ITEM_X;

	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void FieldItem::SetColor()
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuff->Unlock();
}

//=============================================================================
// ��ʓ�����
//=============================================================================
void FieldItem::OnCamera()
{
	CAMERA *camera = GetCamera();

	// ��ʓ�����
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
