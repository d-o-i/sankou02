//=============================================================================
//
// �X�e�[�W�Z���N�g��ʂ̔w�i [StageSelectBG.h]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "StageSelectBG.h"
#include "ResourceManager.h"
#include "MyLibrary.h"
#include "Input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGESELECTBG_DIVIDE_X	(1)
#define STAGESELECTBG_DIVIDE_Y	(3)

int StageSelectBG::PatternAnim = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StageSelectBG::StageSelectBG()
{
	ResourceManager::Instance()->GetTexture("StageSelectBG", &D3DTexture);

	pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	PatternAnim = 0;
	use = true;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
StageSelectBG::~StageSelectBG()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void StageSelectBG::Update()
{
	if (use == true)
	{
		// �X�e�[�W�؂�ւ�����
		for (int playerNo = 0; playerNo < GAMEPAD_MAX; playerNo++)
		{
			if (GetKeyboardRepeat(DIK_S) || IsButtonRepeated(playerNo, STICK_DOWN))
			{
				PatternAnim = LoopCountUp(PatternAnim, 0, STAGESELECTBG_DIVIDE_Y - 1);
				break;
			}
			else if (GetKeyboardRepeat(DIK_W) || IsButtonRepeated(playerNo, STICK_UP))
			{
				PatternAnim = LoopCountDown(PatternAnim, 0, STAGESELECTBG_DIVIDE_Y - 1);
				break;
			}
		}

		SetTexture();

		SetVertex();
	}
}

//=============================================================================
// �`��
//=============================================================================
void StageSelectBG::Draw()
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
// ���_���̍쐬
//=============================================================================
void StageSelectBG::MakeVertex()
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
	SetTexture();
}

//=============================================================================
// �e�N�X�`�����W�̃Z�b�g
//=============================================================================
void StageSelectBG::SetTexture()
{
	int x = PatternAnim % STAGESELECTBG_DIVIDE_X;
	int y = PatternAnim / STAGESELECTBG_DIVIDE_X;
	float sizeX = 1.0f / STAGESELECTBG_DIVIDE_X;
	float sizeY = 1.0f / STAGESELECTBG_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̃Z�b�g
//=============================================================================
void StageSelectBG::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);
}