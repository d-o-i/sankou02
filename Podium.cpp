//=============================================================================
//
// �\����[Podium.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Podium.h"

#define PODIUM_MODEL	("data/MODEL/Podium/Podium.x")
#define PODIUM_ROT		D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Podium::Podium()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this->Load_xFile(PODIUM_MODEL, "Podium");

	// �A�j���[�V�����Z�b�g�̏�����
	this->CreateAnimSet();

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = PODIUM_ROT;
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Podium::~Podium()
{
}

//=============================================================================
// �X�V
//=============================================================================
void Podium::Update()
{

}

//=============================================================================
// �`��
//=============================================================================
void Podium::Draw()
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
void Podium::CreateAnimSet()
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
HRESULT CALLBACK Podium::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}
