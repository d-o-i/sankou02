//=============================================================================
//
// �^�C�g����ʏ��� [TitleCursor.cpp]
// Author :  
//
//=============================================================================
#include "TitleCursor.h"
#include "Main.h"
#include "ResourceManager.h"
#include "SceneTitle.h"
#include "Input.h"
#include "Player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_CURSOR_SIZE			D3DXVECTOR3(400.0f * 0.75f, SCREEN_CENTER_Y*0.50f, 0.0f)// �e�N�X�`���T�C�Y
#define TITLE_CURSOR_POS_START		D3DXVECTOR3(400.0f, SCREEN_CENTER_Y * 1.50f, 0.0f)		// �e�N�X�`�����W
#define TITLE_CURSOR_POS_EXIT		D3DXVECTOR3(250.0f, SCREEN_CENTER_Y * 1.55f, 0.0f)		// �e�N�X�`�����W

//=============================================================================
// �R���X�g���N�^
//=============================================================================
TITLECURSOR::TITLECURSOR()
{
	ResourceManager::Instance()->GetTexture("TitleCursor", &D3DTexture);
	size = TITLE_CURSOR_SIZE;
	pos = TITLE_CURSOR_POS_START;

	use = true;
	PatternAnim = 1;

	// ���_���̍쐬
	MakeVertex();

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
TITLECURSOR::~TITLECURSOR()
{
	// ���\�[�X�̊J���̓��\�[�X�}�l�[�W���ɔC���邽�߁A�Q�Ƃ���߂邾��
	D3DTexture = NULL;

}
//=============================================================================
// �X�V����
//=============================================================================
void  TITLECURSOR::Update()
{

	if (use == true)
	{
		// �e�N�X�`�����W���Z�b�g
		SetTexture(PatternAnim);

		for (int playerNo = 0; playerNo < PLAYER_MAX; playerNo++)
		{
			if (GetKeyboardTrigger(DIK_W) || IsButtonTriggered(playerNo, STICK_UP))
			{

				SetVertex();

			}
			if (GetKeyboardTrigger(DIK_S) || IsButtonTriggered(playerNo, STICK_DOWN))
			{
				SetVertexMove(TITLE_CURSOR_POS_EXIT);
			}

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TITLECURSOR::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�i�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s���j
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����\��Ȃ����Ƃ��w�肷��pDevice->SetTexture(0,NULL);
		pDevice->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT TITLECURSOR::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
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
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;

}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void TITLECURSOR::SetTexture(int cntPattern)
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
void TITLECURSOR::SetVertex(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}

//=============================================================================
// ���_���W�̈ړ�
//=============================================================================
void TITLECURSOR::SetVertexMove(D3DXVECTOR3 pos)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);
}
