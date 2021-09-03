//=============================================================================
//
// �L�����N�^�[�Z���N�g�J�[�\�� [CursorObj.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "CursorObj.h"
#include "Player.h"
#include "Input.h"
#include "_2dobj.h"
#include "MyLibrary.h"
#include "Sound.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CURSOROBJ_SIZE		D3DXVECTOR3(114.0f, 114.0f, 0.0f)	// �e�N�X�`���T�C�Y
#define CURSOROBJ_POS		D3DXVECTOR3(350.0f, 100.0f, 0.0f)
#define CURSOROBJ_SPACE		D3DXVECTOR3(150.0f, 150.0f, 0.0f)	// �\���Ԋu

//*****************************************************************************
// �����o�ϐ��̏�����
//*****************************************************************************
LPDIRECT3DTEXTURE9	CursorObj::D3DTexture = NULL;				// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9	CursorObj::D3DTexture2 = NULL;				// �e�N�X�`���̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CursorObj::CursorObj(int playerNo, int cursorNo)
{
	ResourceManager::Instance()->GetTexture("SelectCursor", &D3DTexture);

	this->playerNo = playerNo;
	this->cursorNo = cursorNo;
	this->selectNo = cursorNo;
	use = true;
	pos.x = cursorNo * CURSOROBJ_SPACE.x + CURSOROBJ_POS.x;
	pos.y = playerNo * CURSOROBJ_SPACE.y + CURSOROBJ_POS.y;
	pos.z = 0.0f;
	
	// ���_���̍쐬
	MakeVertex();
	MakeVert();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CursorObj::~CursorObj()
{
	// ���\�[�X�̊J���̓��\�[�X�}�l�[�W���ɔC����̂ŁA�Q�Ƃ���߂邾��
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void CursorObj::Update()
{
	if (use == true)
	{
		// �ړ�
		Move();

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CursorObj::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`�����W���Z�b�g
	SetTexture();
	SetVertex();

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}
void CursorObj::Draw2()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`�����W���Z�b�g
	SetTex();
	SetVert();

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, D3DTexture2);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT CursorObj::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
	SetTexture();

	return S_OK;
}
HRESULT CursorObj::MakeVert(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVert();

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
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CursorObj::SetTexture()
{
	int x = this->selectNo;
	float sizeX = 1.0f / (float)CURSOROBJ_DIVIDE_X;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 0.5f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.5f);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CursorObj::SetTex()
{
	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void CursorObj::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + CURSOROBJ_SIZE.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + CURSOROBJ_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + CURSOROBJ_SIZE.x, pos.y + CURSOROBJ_SIZE.y, pos.z);
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void CursorObj::SetVert()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f + CURSOROBJ_SIZE.x*5, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f, pos.y + CURSOROBJ_SIZE.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x*10 - 3155.0f + CURSOROBJ_SIZE.x*5, pos.y + CURSOROBJ_SIZE.y, pos.z);
}

//=============================================================================
// �ړ�
//=============================================================================
void CursorObj::Move()
{
	// �E�Ɉړ�
	if (GetKeyboardRepeat(DIK_D) || IsButtonRepeated(this->playerNo, STICK_RIGHT))
	{
		PlaySound(SE_SELECT);
		this->selectNo = LoopCountDown(this->selectNo, 0, CURSOROBJ_MAX - 1);
	}

	// ���Ɉړ�
	if (GetKeyboardRepeat(DIK_A) || IsButtonRepeated(this->playerNo, STICK_LEFT))
	{
		PlaySound(SE_SELECT);
		this->selectNo = LoopCountUp(this->selectNo, 0, CURSOROBJ_MAX - 1);
	}
}