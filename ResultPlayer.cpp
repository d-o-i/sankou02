//=============================================================================
//
// ���U���g��ʂł̃v���C���[ [ResultPlayer.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "ResultPlayer.h"
#include "Player.h"
#include "ResourceManager.h"
#include "SceneCharacterSelect.h"

//*****************************************************************************
// �f�[�^��`
//*****************************************************************************
D3DXVECTOR3 ResultPos[PLAYER_MAX] = {
	D3DXVECTOR3(0.0f, 60.0f, 0.0f),
	D3DXVECTOR3(-100.0f, 40.0f, 0.0f),
	D3DXVECTOR3(80.0f, 30.0f, 0.0f),
	D3DXVECTOR3(160.0f, 0.0f, 0.0f)
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ResultPlayer::ResultPlayer(int rank, int owner)
{
	// xFile��ǂݍ���
	this->Load_xFile(CharaModel[SceneCharacterSelect::GetSelectCharacter(owner)], "Player");

	// �A�j���[�V�����Z�b�g��ݒu����
	this->CreateAnimSet();

	// ���݂̃A�j���[�V���������ʂɂ���Č��肷��
	switch (rank)
	{
	case 0:
		this->ChangeAnim(Victory);
		break;
	case 1:
	case 2:
		this->ChangeAnim(Clapping);
		break;
	case 3:
		this->ChangeAnim(Lose);
		break;
	default:
		break;
	}

	pos = ResultPos[rank];
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ResultPlayer::~ResultPlayer()
{
}

//=============================================================================
// �X�V
//=============================================================================
void ResultPlayer::Update()
{
	UpdateAnim(TIME_PER_FRAME);
}

//=============================================================================
// �`��
//=============================================================================
void ResultPlayer::Draw()
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
// �A�j���[�V�����̃R�[���o�b�N
//=============================================================================
HRESULT CALLBACK ResultPlayer::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	return S_OK;
}

//=============================================================================
// �A�j���[�V�����Z�b�g�̍쐬
//=============================================================================
void ResultPlayer::CreateAnimSet()
{
	ANIMATIONSET *AnimationSet = new ANIMATIONSET();
	vector<KEYDATA>Keydata;
	Keydata.reserve(Keydata_Max);

	for (int Set_No = 0; Set_No < GetAnimSetNum(); Set_No++)
	{
		switch (Set_No)
		{
		case Idle:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Idle", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Running:

			//Keydata.push_back(KEYDATA{ 0.95f,e_MotionEnd });
			AnimationSet->SetData("Running", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Jump:

			//Keydata.push_back(KEYDATA{ 0.80f, e_MotionEnd });
			AnimationSet->SetData("Jump", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Victory:

			AnimationSet->SetData("Victory", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Slip:

			//Keydata.push_back(KEYDATA{ 0.95f, e_MotionChange });
			AnimationSet->SetData("Slip", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Stop:

			AnimationSet->SetData("Stop", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Lose:

			AnimationSet->SetData("Lose", NULL, 1.5f, 0.1f, 0.0f);
			break;

		case Clapping:

			AnimationSet->SetData("Clapping", NULL, 1.5f, 0.1f, 0.0f);
			break;

		default:
			break;
		}

		this->SetupCallbackKeys(&Keydata, AnimationSet->GetSetName());

		AnimationSet->SetAnimSetPtr(this->AnimController);

		this->AnimSet.push_back(*AnimationSet);

		Keydata.clear();
	}

	SAFE_DELETE(AnimationSet);
	ReleaseVector(Keydata);

}