//=============================================================================
//
// �o�g����ʃt���[���\������ [Faceframe.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Faceframe.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FACEFRAME_SIZE		D3DXVECTOR3(100.0f, 88.0f, 0.0f)		// �e�N�X�`���T�C�Y
#define FACEFRAME_POS		D3DXVECTOR3(10.0f, 10.0f, 0.0f)
#define FACEFRAME_INTERVAL	(318.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
FaceFrame::FaceFrame(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("FaceFrame", &D3DTexture);

	pos = FACEFRAME_POS + D3DXVECTOR3(FACEFRAME_INTERVAL * PlayerNo, 0.0f, 0.0f);

	PatternAnim = 1;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
FaceFrame::~FaceFrame()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void FaceFrame::Update()
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void FaceFrame::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);
		
	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT FaceFrame::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex();

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
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void FaceFrame::SetPlayerDeadTexture(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.5f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.5f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void FaceFrame::SetVertex(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + FACEFRAME_SIZE.x, pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + FACEFRAME_SIZE.y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + FACEFRAME_SIZE.x, pos.y + FACEFRAME_SIZE.y, 0.0f);
}
