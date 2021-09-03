//=============================================================================
//
// �C���N�Q�[�W�\������ [Ink.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "InkGauge.h"
#include "PaintManager.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INKGAUGE_DIVIDE_X		(1)
#define INKGAUGE_DIVIDE_Y		(5)

#define InkGauge_Size			D3DXVECTOR2(120.0f, 30.0f)	// �\���T�C�Y
#define ColorInkGauge_Pos		D3DXVECTOR3(86.0f, 35.0f, 0.0f)
#define BlackInkGauge_Pos		D3DXVECTOR3(86.0f, 45.0f, 0.0f)
#define InkGauge_Interval		(318.0f)

#define GaugeFrame_Size			D3DXVECTOR3(200.0f, 88.0f, 0.0f)		// �e�N�X�`���T�C�Y
#define GaugeFrame_Pos			D3DXVECTOR3(10.0f, 10.0f, 0.0f)
#define GaugeFrame_Interval		(318.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
InkGauge::InkGauge(int InkType, int PlayerNo)
{
	// �C���N�̏�����
	use = true;
	this->InkType = InkType;
	this->Owner = PlayerNo;

	if (this->InkType == ColorInk)
	{
		ResourceManager::Instance()->GetTexture("ColorInkFrame", &Texture_GaugeFrame);
	}
	else if (this->InkType == BlackInk)
	{
		ResourceManager::Instance()->GetTexture("BlackInkFrame", &Texture_GaugeFrame);
	}

	ResourceManager::Instance()->GetTexture("InkGauge", &Texture_InkGauge);

	// �C���N�̒��_���
	MakeVertex_GaugeFrame(INK_MAX);
	// �C���N�p�t���[���̒��_���
	MakeVertex_InkGauge(INK_MAX);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
InkGauge::~InkGauge()
{
	// �e�N�X�`���̊J��
	Texture_InkGauge = NULL;
	Texture_GaugeFrame = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void InkGauge::Update(int InkValue)
{
	if (use == true)
	{
		// ���_���W�̐ݒ�
		SetVertex_InkGauge(InkValue);
		//�e�N�X�`�����W���Z�b�g
		SetTexture_InkGauge(InkValue);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void InkGauge::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (use)
	{
		Device->SetTexture(0, Texture_InkGauge);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GaugeVertex, sizeof(Vertex2D));

		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		Device->SetTexture(0, Texture_GaugeFrame);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, FrameVertex, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT InkGauge::MakeVertex_GaugeFrame(int InkValue)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex_GaugeFrame(InkValue);

	// rhw�̐ݒ�
	FrameVertex[0].rhw = 1.0f;
	FrameVertex[1].rhw = 1.0f;
	FrameVertex[2].rhw = 1.0f;
	FrameVertex[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	FrameVertex[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	FrameVertex[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTexture_GaugeFrame(InkValue);

	return S_OK;
}

HRESULT InkGauge::MakeVertex_InkGauge(int InkValue)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex_InkGauge(InkValue);

	// rhw�̐ݒ�
	GaugeVertex[0].rhw = 1.0f;
	GaugeVertex[1].rhw = 1.0f;
	GaugeVertex[2].rhw = 1.0f;
	GaugeVertex[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	GaugeVertex[0].rhw = 1.0f;
	GaugeVertex[1].rhw = 1.0f;
	GaugeVertex[2].rhw = 1.0f;
	GaugeVertex[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	SetTexture_InkGauge(InkValue);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void InkGauge::SetTexture_GaugeFrame(int InkValue)
{
	// �e�N�X�`�����W�̐ݒ�
	FrameVertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	FrameVertex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	FrameVertex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	FrameVertex[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

void InkGauge::SetTexture_InkGauge(int InkValue)
{
	int y = 0;
	if (InkType == BlackInk)
	{
		y = BlackInkColor;
	}
	else if (InkType == ColorInk)
	{
		y = Owner;
	}
	float sizeY = 1.0f / INKGAUGE_DIVIDE_Y;
	float InkPercent = (float)InkValue / INK_MAX;

	// �e�N�X�`�����W�̐ݒ�
	GaugeVertex[0].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY);
	GaugeVertex[1].tex = D3DXVECTOR2(1.0f * InkPercent, (float)(y)* sizeY);
	GaugeVertex[2].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY + sizeY);
	GaugeVertex[3].tex = D3DXVECTOR2(1.0f * InkPercent, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void InkGauge::SetVertex_GaugeFrame(int InkValue)
{
	D3DXVECTOR3 pos = GaugeFrame_Pos + D3DXVECTOR3(this->Owner * GaugeFrame_Interval, 0.0f, 0.0f);

	// ���_���W�̐ݒ�
	FrameVertex[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	FrameVertex[1].vtx = D3DXVECTOR3(pos.x + GaugeFrame_Size.x, pos.y, 0.0f);
	FrameVertex[2].vtx = D3DXVECTOR3(pos.x, pos.y + GaugeFrame_Size.y, 0.0f);
	FrameVertex[3].vtx = D3DXVECTOR3(pos.x + GaugeFrame_Size.x, pos.y + GaugeFrame_Size.y, 0.0f);
}

void InkGauge::SetVertex_InkGauge(int InkValue)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float InkPercent = (float)InkValue / INK_MAX;

	if (this->InkType == ColorInk)
	{
		pos = ColorInkGauge_Pos + D3DXVECTOR3(this->Owner * InkGauge_Interval, 0.0f, 0.0f);
	}
	else if (this->InkType == BlackInk)
	{
		pos = BlackInkGauge_Pos + D3DXVECTOR3(this->Owner * InkGauge_Interval, 0.0f, 0.0f);
	}

	// ���_���W�̐ݒ�
	GaugeVertex[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	GaugeVertex[1].vtx = D3DXVECTOR3(pos.x + InkGauge_Size.x * InkPercent, pos.y, 0.0f);
	GaugeVertex[2].vtx = D3DXVECTOR3(pos.x, pos.y + InkGauge_Size.y, 0.0f);
	GaugeVertex[3].vtx = D3DXVECTOR3(pos.x + InkGauge_Size.x * InkPercent, pos.y + InkGauge_Size.y, 0.0f);
}


