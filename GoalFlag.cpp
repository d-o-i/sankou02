//=============================================================================
//
// �S�[���n�_�\�� [GoalFlag.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "GoalFlag.h"
#include "Map.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "StageSelectBG.h"
#include "DebugWindow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GOAL_SIZE		D3DXVECTOR3(12.8f, 12.8f, 0.0f)
#define FLAG_POS		D3DXVECTOR3(GOAL_POS.x, -88.0f, -10.0f)

D3DXVECTOR3 GoalFlagPos[] = {
	D3DXVECTOR3(GOAL_POS.x, -530.0f, 0.0f),
	D3DXVECTOR3(GOAL_POS.x, -270.0f, 0.0f),
	D3DXVECTOR3(GOAL_POS.x, -470.0f, 0.0f),
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
GoalFlag::GoalFlag()
{
	ResourceManager::Instance()->GetTexture("GoalFlag", &D3DTexture);

	pos = GoalFlagPos[StageSelectBG::GetStageSelect()];
	scl = GOAL_SIZE;
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
GoalFlag::~GoalFlag()
{
	D3DTexture = NULL;
	SAFE_RELEASE(D3DVtxBuff);
}

//=============================================================================
// �X�V
//=============================================================================
void GoalFlag::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void GoalFlag::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *cameraWk = GetCamera();
	D3DXMATRIX WorldMtx, ViewMtx, SclMtx, TransMtx;

	// Z�e�X�g
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

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
	Device->SetTexture(0, D3DTexture);

	// �|���S���̕`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	//Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT GoalFlag::MakeVertex()
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
	pVtx[0].vtx = D3DXVECTOR3(-GOAL_SIZE.x / 2, GOAL_SIZE.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(GOAL_SIZE.x / 2, GOAL_SIZE.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-GOAL_SIZE.x / 2, -GOAL_SIZE.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(GOAL_SIZE.x / 2, -GOAL_SIZE.y / 2, 0.0f);

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

	return S_OK;
}