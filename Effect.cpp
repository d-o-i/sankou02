//=============================================================================
//
// �G�t�F�N�g���� [Effect.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Effect.h"
#include "MyLibrary.h"
#include "ResourceManager.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �G�t�F�N�g�f�[�^�\����
struct EffectData
{
	const char *texture;	// �e�N�X�`���t�@�C��
	D3DXVECTOR3 size;		// �T�C�Y
	int count;				// �X�V�t���[��
	Int2D pattern;			// �e�N�X�`��������(x,y)
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static EffectData EffectDataWk[EffectMax] =
{
	{ "data/EFFECT/Hit.png", D3DXVECTOR3(250.0f, 250.0f, 0.0f), 4, Int2D(2, 5) },
{ "data/EFFECT/anmef001.png", D3DXVECTOR3(500.0f, 100.0f, 0.0f), 4, Int2D(1, 5) },
{ "data/EFFECT/anmef002.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 4, Int2D(2, 2) },
{ "data/EFFECT/explo000.png", D3DXVECTOR3(75.0f, 75.0f, 0.0f), 4, Int2D(5, 3) },
{ "data/EFFECT/ief001.png", D3DXVECTOR3(100.0f, 100.0f, 0.0f), 10, Int2D(5, 2) },
{ "data/EFFECT/ief000.png", D3DXVECTOR3(70.0f, 70.0f, 0.0f), 4, Int2D(3, 1) },
{ "data/EFFECT/Charge.png", D3DXVECTOR3(75.0f, 75.0f, 0.0f), 10 ,Int2D(2, 7) },
};

//=============================================================================
// �R���X�g���N�^(�������[�v������ꍇ��INFINITY_LOOP��n��)
//=============================================================================
Effect::Effect(EffectNum num, D3DXVECTOR3 _pos, int _LoopNum)
{
	
	D3DVtxBuff = NULL;

	TexNo = num;

	switch (TexNo)
	{
	case HitEffect:
		ResourceManager::Instance()->GetTexture("HitEffect", &D3DTexture);
		break;
	case DeadEffect:
		ResourceManager::Instance()->GetTexture("DeadEffect", &D3DTexture);
		break;
	case RunEffect:
		ResourceManager::Instance()->GetTexture("RunEffect", &D3DTexture);
		break;
	case ExplosionEffect:
		ResourceManager::Instance()->GetTexture("ExplosionEffect", &D3DTexture);
		break;
	case ItemEffect:
		ResourceManager::Instance()->GetTexture("ItemEffect", &D3DTexture);
		break;
	case Item1Effect:
		ResourceManager::Instance()->GetTexture("Item1Effect", &D3DTexture);
		break;
	case ChargeEffect:
		ResourceManager::Instance()->GetTexture("ChargeEffect", &D3DTexture);
		break;
	default:
		break;
	}

	// �t���[���̏�����
	use = true;
	CountAnim = 0;
	PatternAnim = 0;
	size = EffectDataWk[TexNo].size;
	pos = _pos;
	xPattern = EffectDataWk[TexNo].pattern.x;
	yPattern = EffectDataWk[TexNo].pattern.y;
	TexAnimNum = xPattern * yPattern;
	AnimationCnt = EffectDataWk[TexNo].count;
	loopnum = _LoopNum;
	loopcnt = 0;

	// ���_���̍쐬
	MakeVertex();

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Effect::~Effect()
{
	if (D3DVtxBuff != NULL)
	{
		D3DVtxBuff->Release();
		D3DVtxBuff = NULL;
	}

	D3DTexture = NULL;
}

//=============================================================================
// �X�V
//=============================================================================
void Effect::Update()
{
	if (use == true)
	{
		// �A�j���[�V����
		CountAnim++;

		if ((CountAnim % AnimationCnt) == 0)
		{
			// �p�^�[���̐؂�ւ�
			PatternAnim = LoopCountUp(PatternAnim, 0, TexAnimNum);

			// ���[�v����
			Loop();

			//�e�N�X�`�����W���Z�b�g
			SetTexture(PatternAnim);

		}

	}

	// ���_���W�̐ݒ�
	SetVertex();

}

//=============================================================================
// �`��
//=============================================================================
void Effect::Draw() {

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
// �G�t�F�N�g�̃��[�v����
//=============================================================================
void Effect::Loop()
{

	// �������[�v�̂Ƃ��ȊO
	if (loopnum != INFINITY_LOOP)
	{
		if (PatternAnim == 0)
		{
			loopcnt++;

			// �w��̃��[�v�񐔂ɒB������G�t�F�N�g���I��
			if (loopcnt == loopnum)
			{
				use = false;
				loopcnt = 0;
			}
		}
	}

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT Effect::MakeVertex(void) {

	// ���_���W�̐ݒ�
	SetVertex();

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
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
void Effect::SetTexture(int cntPattern) {

	int x = cntPattern % xPattern;
	int y = cntPattern / xPattern;
	float sizeX = 1.0f / xPattern;
	float sizeY = 1.0f / yPattern;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void Effect::SetVertex(void) {

	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + size.x, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + size.y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, pos.z);

}