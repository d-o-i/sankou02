//=============================================================================
//
// �����i�[ [Runner.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Runner.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include "MyLibrary.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RUNNER_SIZE			D3DXVECTOR3(380.0f, 380.0f, 0.0f)	// �e�N�X�`���T�C�Y
#define RUNNER_FIRST_POS	D3DXVECTOR3(-SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.9f, 0.0f)
#define RUNNER_POS			D3DXVECTOR3(SCREEN_CENTER_X * 1.4f, SCREEN_CENTER_Y * 0.9f, 0.0f)		// �e�N�X�`�����W
#define RUNNER_MOVE_VALUE	(50.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Runner::Runner()
{
	ResourceManager::Instance()->GetTexture("TitleRunner", &D3DTexture);
	size = RUNNER_SIZE;
	pos = RUNNER_FIRST_POS;
	use = true;
	PatternAnim = 1;

	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Runner::~Runner()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Runner::Update()
{
	if (!use)
		return;

	if (pos.x < RUNNER_POS.x)
	{
		pos.x += RUNNER_MOVE_VALUE;

		if (pos.x >= RUNNER_POS.x)
		{
			// �G�t�F�N�g����
			std::vector<UIParticle*> *particle = ParticleManager::GetUIParticle();

			D3DXVECTOR3 move;

			for (int i = 0; i < 100; i++)
			{
				move.x = RandomRange(-30.0f, 30.0f);
				move.y = RandomRange(-30.0f, 30.0f);
				particle->push_back(new UIParticle(pos, move, MyColor[nWhite], D3DXVECTOR3(10.0f, 10.0f, 0.0f), 120));
			}
		}
	}

	SetVertex();
}

//=============================================================================
// �`��
//=============================================================================
void Runner::Draw()
{
	if (!use)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, D3DTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void Runner::MakeVertex()
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

}

//=============================================================================
// ���_���W�̃Z�b�g
//=============================================================================
void Runner::SetVertex()
{
	vertexWk[0].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

}