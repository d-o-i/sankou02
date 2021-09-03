//=============================================================================
//
// �L�����N�^�[�Z���N�g���S [SelectLogo.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "carslobj.h"
#include "SelectLogo.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SELECTLOGO_SIZE		D3DXVECTOR3(1280.0f, 114.0f, 0.0f)		// �e�N�X�`���T�C�Y

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SelectLogo::SelectLogo()
{
	ResourceManager::Instance()->GetTexture("SelectLogo", &D3DTexture);

	use = true;
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PatternAnim = 1;
	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SelectLogo::~SelectLogo()
{
	// ���\�[�X�̊J���̓��\�[�X�}�l�[�W���ɔC����̂ŁA�Q�Ƃ���߂邾��
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void SelectLogo::Update()
{
	if (use == true)
	{

		//�e�N�X�`�����W���Z�b�g
		SetTexture(PatternAnim);

		SetVertex();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void SelectLogo::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT SelectLogo::MakeVertex(void)
{
	// ���_���W�̐ݒ�
	SetVertex();

	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTexture(PatternAnim);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SelectLogo::SetTexture(int cntPattern)
{
	int x = cntPattern;
	int y = cntPattern;
	float sizeX = 1.0f;
	float sizeY = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SelectLogo::SetVertex(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + SELECTLOGO_SIZE.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + SELECTLOGO_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + SELECTLOGO_SIZE.x, pos.y + SELECTLOGO_SIZE.y, pos.z);
}
