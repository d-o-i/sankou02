//=============================================================================
//
// �L�����N�^�[�Z���N�gUI [CharSelectUI.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "CharSelectUI.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���T�C�Y
#define PlayerNumTex_Size	D3DXVECTOR3(192.0f, 96.0f, 0.0f)
#define ModelTex_Size		D3DXVECTOR3(96.0f, 96.0f, 0.0f)
#define CPUIconTex_Size		D3DXVECTOR3(100.0f, 40.0f, 0.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CharSelectUI::CharSelectUI(int PlayerNo, bool AIUse)
{
	ResourceManager::Instance()->GetTexture("ResultRank", &PlayerNumTex);
	ResourceManager::Instance()->GetTexture("SelectCursor", &ModelTex);
	ResourceManager::Instance()->GetTexture("CPUIcon", &CPUIconTex);

	this->AIUse = AIUse;
	this->PlayerNo = PlayerNo;
	// ����
	if (PlayerNo == 0)
	{
		this->Pos = D3DXVECTOR3(70.0f, 220.0f, 0.0f);
	}
	// �E��
	else if (PlayerNo == 1)
	{
		this->Pos = D3DXVECTOR3(980.0f, 220.0f, 0.0f);
	}
	// ����
	else if (PlayerNo == 2)
	{
		this->Pos = D3DXVECTOR3(70.0f, 500.0f, 0.0f);
	}
	// �E��
	else if (PlayerNo == 3)
	{
		this->Pos = D3DXVECTOR3(980.0f, 500.0f, 0.0f);
	}

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CharSelectUI::~CharSelectUI()
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CharSelectUI::Update()
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CharSelectUI::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetFVF(FVF_VERTEX_2D);
	Device->SetTexture(0, PlayerNumTex);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, PlayerNumVtx, sizeof(Vertex2D));

	if (SelectOver)
	{
		Device->SetTexture(0, ModelTex);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ModelVtx, sizeof(Vertex2D));
	}

	if (AIUse)
	{
		Device->SetTexture(0, CPUIconTex);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, CPUIconVtx, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void CharSelectUI::MakeVertex(void)
{
	// ���_���W�̐ݒ�
	PlayerNumVtx[0].vtx = D3DXVECTOR3(Pos.x - PlayerNumTex_Size.x / 2, Pos.y - PlayerNumTex_Size.y / 2, 0.0f);
	PlayerNumVtx[1].vtx = D3DXVECTOR3(Pos.x + PlayerNumTex_Size.x / 2, Pos.y - PlayerNumTex_Size.y / 2, 0.0f);
	PlayerNumVtx[2].vtx = D3DXVECTOR3(Pos.x - PlayerNumTex_Size.x / 2, Pos.y + PlayerNumTex_Size.y / 2, 0.0f);
	PlayerNumVtx[3].vtx = D3DXVECTOR3(Pos.x + PlayerNumTex_Size.x / 2, Pos.y + PlayerNumTex_Size.y / 2, 0.0f);

	D3DXVECTOR3 ModelPos = D3DXVECTOR3(Pos.x + 120.0f, Pos.y, 0.0f);
	ModelVtx[0].vtx = D3DXVECTOR3(ModelPos.x - ModelTex_Size.x / 2, ModelPos.y - ModelTex_Size.y / 2, 0.0f);
	ModelVtx[1].vtx = D3DXVECTOR3(ModelPos.x + ModelTex_Size.x / 2, ModelPos.y - ModelTex_Size.y / 2, 0.0f);
	ModelVtx[2].vtx = D3DXVECTOR3(ModelPos.x - ModelTex_Size.x / 2, ModelPos.y + ModelTex_Size.y / 2, 0.0f);
	ModelVtx[3].vtx = D3DXVECTOR3(ModelPos.x + ModelTex_Size.x / 2, ModelPos.y + ModelTex_Size.y / 2, 0.0f);

	D3DXVECTOR3 IconPos = D3DXVECTOR3(Pos.x, Pos.y + 50.0f, 0.0f);
	CPUIconVtx[0].vtx = D3DXVECTOR3(IconPos.x - CPUIconTex_Size.x / 2, IconPos.y - CPUIconTex_Size.y / 2, 0.0f);
	CPUIconVtx[1].vtx = D3DXVECTOR3(IconPos.x + CPUIconTex_Size.x / 2, IconPos.y - CPUIconTex_Size.y / 2, 0.0f);
	CPUIconVtx[2].vtx = D3DXVECTOR3(IconPos.x - CPUIconTex_Size.x / 2, IconPos.y + CPUIconTex_Size.y / 2, 0.0f);
	CPUIconVtx[3].vtx = D3DXVECTOR3(IconPos.x + CPUIconTex_Size.x / 2, IconPos.y + CPUIconTex_Size.y / 2, 0.0f);

	// rhw�̐ݒ�
	PlayerNumVtx[0].rhw = 1.0f;
	PlayerNumVtx[1].rhw = 1.0f;
	PlayerNumVtx[2].rhw = 1.0f;
	PlayerNumVtx[3].rhw = 1.0f;

	ModelVtx[0].rhw = 1.0f;
	ModelVtx[1].rhw = 1.0f;
	ModelVtx[2].rhw = 1.0f;
	ModelVtx[3].rhw = 1.0f;

	CPUIconVtx[0].rhw = 1.0f;
	CPUIconVtx[1].rhw = 1.0f;
	CPUIconVtx[2].rhw = 1.0f;
	CPUIconVtx[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	PlayerNumVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	PlayerNumVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	PlayerNumVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	PlayerNumVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	ModelVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	ModelVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	ModelVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	ModelVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	CPUIconVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CPUIconVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CPUIconVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	CPUIconVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	float SizeY = 1.0f / 4;
	PlayerNumVtx[0].tex = D3DXVECTOR2(0.0f, PlayerNo* SizeY);
	PlayerNumVtx[1].tex = D3DXVECTOR2(1.0f, PlayerNo* SizeY);
	PlayerNumVtx[2].tex = D3DXVECTOR2(0.0f, PlayerNo* SizeY + SizeY);
	PlayerNumVtx[3].tex = D3DXVECTOR2(1.0f, PlayerNo* SizeY + SizeY);

	ModelVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ModelVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ModelVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ModelVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	CPUIconVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	CPUIconVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	CPUIconVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	CPUIconVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CharSelectUI::SetCharTexture(int CharacterNo)
{
	float SizeX = 1.0f / 4;

	// �e�N�X�`�����W�̐ݒ�
	ModelVtx[0].tex = D3DXVECTOR2(CharacterNo * SizeX, 0.0f);
	ModelVtx[1].tex = D3DXVECTOR2(CharacterNo * SizeX + SizeX, 0.0f);
	ModelVtx[2].tex = D3DXVECTOR2(CharacterNo * SizeX, 0.5f);
	ModelVtx[3].tex = D3DXVECTOR2(CharacterNo * SizeX + SizeX, 0.5f);

	SelectOver = true;
}
