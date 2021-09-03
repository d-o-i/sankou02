//=============================================================================
//
// �y�C���g [Paint.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Paint.h"
#include "Camera.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

#define DecAlpha (0.1f)

//*****************************************************************************
// �����o�̏�����
//*****************************************************************************
float Paint::HalfSize = PAINT_WIDTH / 2;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Paint::Paint(int Owner, int PaintColor)
{
	ResourceManager::Instance()->GetTexture("Paint", &D3DTexture);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	width = PAINT_WIDTH;
	height = PAINT_HEIGHT;
	time = 0;
	this->Use = false;
	this->InScreen = true;
	this->NodeID = 0;
	this->Owner = Owner;
	this->PaintColor = PaintColor;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Paint::~Paint()
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(this->D3DVtxBuff);

	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void Paint::Update()
{
	// �g�p���Ă�����̂̂ݍX�V
	if (Use)
	{
		// �\�����Ԃ̍X�V
		if (time > 0)
		{
			time--;
		}
		// �\�����Ԃ𒴂�������ŏ���
		else
		{
			// �����x�������l�ɍ��킹�Ēǉ�
			col.a -= DecAlpha;

			if (col.a <= 0.0f)
			{
				// �����ɂȂ�����g�p����߂�
				col.a = 0.0f;
				Use = false;
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �F�̐ݒ�
			SetColor();
		}

#if _DEBUG
#endif
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Paint::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

#if 0
	// ���Z���� �����_�����O�X�e�[�g�̕ύX�������ۂ��Ȃ�i���Z�����͔����ۂ��Ȃ�i255�ɋ߂Â��Ă����Ɓj�j
	//Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�e�X�g
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#endif

	// Z�e�X�g
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	//Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	//Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (Use)
	{
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

		// ���݂̃I�u�W�F�N�g����ʓ����m�F
		CheckInScreen(WorldMtx);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(Vertex3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, Paint::D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	// ���e�X�g�𖳌���
	//Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

#if 0
	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Paint::MakeVertex()
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
	pVtx[0].vtx = D3DXVECTOR3(-PAINT_WIDTH / 2, PAINT_HEIGHT / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(PAINT_WIDTH / 2, PAINT_HEIGHT / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-PAINT_WIDTH / 2, -PAINT_HEIGHT / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(PAINT_WIDTH / 2, -PAINT_HEIGHT / 2, 0.0f);

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
	int x = this->PaintColor % PAINT_DIVIDE_X;
	float sizeX = 1.0f / PAINT_DIVIDE_X;

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
void Paint::SetColor()
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
// ���[���h���W����X�N���[�����W�ɕϊ�����
//=============================================================================
void Paint::CheckInScreen(D3DXMATRIX WorldMatrix)
{
	ScreenPos = WorldToScreenPos(WorldMatrix);

	if (ScreenPos.x < 0 ||
		ScreenPos.x > SCREEN_WIDTH ||
		ScreenPos.y < 0 ||
		ScreenPos.y > SCREEN_HEIGHT)
	{
		this->InScreen = false;
	}
	else
	{
		this->InScreen = true;
	}
}