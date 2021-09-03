//=============================================================================
//
// ���U���g��ʂł̌� [ResultDigit.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultDigit.h"
#include "ResultRank.h"
#include "Player.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULTDIGIT_POS			D3DXVECTOR3(450.0f, 178.0f, 0.0f)
#define RESULTDIGIT_SIZE		D3DXVECTOR3(40.0f, 85.0f, 0.0f)
#define RESULTDIGIT_INTERVAL	(80.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ResultDigit::ResultDigit(DWORD _time, int _digit, int _rank) : Digit(_digit)
{
	ResourceManager::Instance()->GetTexture("Digit", &D3DTexture);

	time = _time;
	use = true;
	rank = _rank;
	pos = RESULTDIGIT_POS + D3DXVECTOR3(0.0f, RESULTDIGIT_INTERVAL * rank, 0.0f);
	size = RESULTDIGIT_SIZE;
	pos.x -= _digit * size.x;
	// �~���b�ƕb�A�b�ƕ��̊Ԃ�':'�̃X�y�[�X������
	if (_digit >= 2)
	{
		pos.x -= 20.0f;
	}
	if (_digit >= 4)
	{
		pos.x -= 20.0f;
	}

	MakeVertex();
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
ResultDigit::~ResultDigit()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void ResultDigit::Update()
{
}

//=============================================================================
// �`��
//=============================================================================
void ResultDigit::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// Z�e�X�g
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		Device->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}