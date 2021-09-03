//=============================================================================
//
// �g���t�B�[[Trophy.cpp]
// Author :  
//
//=============================================================================
#include "Main.h"
#include "Trophy.h"

#define TROPHY_MODEL	("data/MODEL/Trophy.x")
#define TROPHY_POS		D3DXVECTOR3(0.0f, 150.0f, 0.0f)
#define TROPHY_ROT		D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(30))
#define TROPHY_MOVE		D3DXVECTOR3(0.0f, D3DXToRadian(1), 0.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Trophy::Trophy()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this->Load_xFile(TROPHY_MODEL, "Podium");

	// �A�j���[�V�����Z�b�g�̏�����
	this->CreateAnimSet();

	pos = TROPHY_POS;
	rot = D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(30));
	scl = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Trophy::~Trophy()
{
}

//=============================================================================
// �X�V
//=============================================================================
void Trophy::Update()
{
	// ��]������
	rot += TROPHY_MOVE;
	if (rot.y >= D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}
}

//=============================================================================
// �`��
//=============================================================================
void Trophy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 matDef;
	D3DXMATRIX WorldMtx, SclMtx, RotMtx, TransMtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&SclMtx, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&SclMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &SclMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �����_�����O
	this->DrawAnim(&WorldMtx);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// �A�j���[�V�����Z�b�g�̍쐬
//=============================================================================
void Trophy::CreateAnimSet()
{
	ANIMATIONSET *AnimationSet = new ANIMATIONSET();
	vector<KEYDATA>Keydata;
	Keydata.reserve(Keydata_Max);
	AnimationSet->SetData("Idle", NULL, 1.0f, 0.1f, 0.0f);
	this->SetupCallbackKeys(&Keydata, AnimationSet->GetSetName());
	AnimationSet->SetAnimSetPtr(this->AnimController);
	this->AnimSet.push_back(*AnimationSet);
	Keydata.clear();
	SAFE_DELETE(AnimationSet);
	ReleaseVector(Keydata);

}

//=====================================================================================================
// �A�j���[�V����CallbackKey�̏����C�x���g
//=====================================================================================================
HRESULT CALLBACK Trophy::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}
