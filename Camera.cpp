//=============================================================================
//
// �J�������� [Camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Camera.h"
#include "Input.h"
#include "SceneManager.h"
#include "DebugWindow.h"
#include "MyLibrary.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAMERA_POS		D3DXVECTOR3(0.0f, 10.0f, -500.0f)				// �J�����̏����ʒu
#define CAMERA_POS_RESULT	D3DXVECTOR3(0.0f, -100.0f, -450.0f)			// ���U���g��ʂł̃J�����̏����ʒu
#define CAMERA_AT		D3DXVECTOR3(0.0f, 0.0f, 0.0f)					// �J�����̒����_
#define CAMERA_AT_RESULT	D3DXVECTOR3(0.0f, 300.0f, 0.0f)
#define CAMERA_AT_TITLE		D3DXVECTOR3(100.0f, -100.0f, 0.0f)

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.002f)							// �J�����̉�]��

#define	INTERVAL_CAMERA_R	(12.5f)										// ���f���̎����̐�܂ł̋���
#define	RATE_CHASE_CAMERA_P	(0.35f)										// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.20f)										// �J�����̒����_�ւ̕␳�W��
#define CAMERA_SPD			(0.05f)										// �Â��J���������_�ƐV�����J���������_�ɍ�������ꍇ�ɃJ�����ʒu���X�V����X�s�[�h

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA cameraWk;

void DebugCamera();

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	cameraWk.pos = CAMERA_POS;
	cameraWk.at = CAMERA_AT;
	if (GetScene() == nSceneResult)
	{
		cameraWk.at = CAMERA_AT_RESULT;
		cameraWk.pos = cameraWk.at + CAMERA_POS_RESULT;
	}
	else if (GetScene() == nSceneTitle)
	{
		cameraWk.at = CAMERA_AT_TITLE;
		cameraWk.pos = cameraWk.at + CAMERA_POS;
	}
	cameraWk.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraWk.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	float vx, vz;
	vx = cameraWk.pos.x - cameraWk.at.x;
	vz = cameraWk.pos.z - cameraWk.at.z;
	cameraWk.distance = sqrtf(vx * vx + vz * vz);
}

//=============================================================================
// �J�����̍X�V�����iSceneGame�p�j
//=============================================================================
void UpdateCamera(D3DXVECTOR3 _at)
{
	// ��ԑO�ɂ���L�����N�^�̍��W��V���������_�Ƃ���
	D3DXVECTOR3 newAt = _at;

	// �X�V�O�̒����_�ƐV���������_�̍����x�N�g���ɂ���
	D3DXVECTOR3 DistVec = newAt - cameraWk.at;

	// ���X�ɐV���������_�ɋ߂Â���
	cameraWk.at += DistVec * CAMERA_SPD;
	
	// ���W�͒����_�ɑ΂��ĕ��s�ړ�����
	cameraWk.pos = cameraWk.at + CAMERA_POS;

}

//=============================================================================
// �J�����̍X�V�����iSceneResult�p�j
//=============================================================================
void UpdateCamera()
{
	static float x = cameraWk.pos.x, y = cameraWk.pos.y, z = cameraWk.pos.z;

	// ����]�����Â���
	cameraWk.rot.y += VALUE_ROTATE_CAMERA;
	if (cameraWk.rot.y > D3DX_PI)
	{
		cameraWk.rot.y = -D3DX_PI;
	}
	x = cameraWk.distance * sinf(cameraWk.rot.y);
	z = cameraWk.distance * cosf(cameraWk.rot.y);

#ifndef _DEBUG_
	// �J������Y����]
	if (GetKeyboardPress(DIK_RIGHT))
	{
		cameraWk.rot.y -= VALUE_ROTATE_CAMERA;
		if (cameraWk.rot.y < -D3DX_PI)
		{
			cameraWk.rot.y = D3DX_PI;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		cameraWk.rot.y += VALUE_ROTATE_CAMERA;
		if (cameraWk.rot.y > D3DX_PI)
		{
			cameraWk.rot.y = -D3DX_PI;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}

	// �J�����̒����_�Ǝ��_�̋����ύX
	if (GetKeyboardPress(DIK_UP))
	{
		if (cameraWk.distance > 5.0f)
		{
			cameraWk.distance -= 5.0f;
		}
		else if (cameraWk.distance == 5.0f)
		{
			cameraWk.distance = 1.0f;
		}
		else if (cameraWk.distance < 5.0f)
		{
			cameraWk.distance = 1.0f;
		}
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		cameraWk.distance += 5.0f;
		x = cameraWk.distance * sinf(cameraWk.rot.y);
		z = cameraWk.distance * cosf(cameraWk.rot.y);
	}

	//DebugCamera();
#endif

	cameraWk.pos = cameraWk.at + D3DXVECTOR3(x, y, z);	// �J�����̈ʒu�i���_�j
	cameraWk.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �J�����̒����_�����[���h�̒��S�_
	cameraWk.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 3D��Ԃ̏�����͂ǂ���H��Y���{��������

}

//=============================================================================
// �^�C�g����ʂł̃J�����̍X�V
//=============================================================================
void UpdateTitleCamera()
{
	if (cameraWk.at.x < 9800.0f)
	{
		cameraWk.at.x += 1.0f;
		cameraWk.at.y -= 0.1f;
	}
	cameraWk.pos = cameraWk.at + CAMERA_POS;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cameraWk.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&cameraWk.mtxView,
		&cameraWk.pos,		// �J�����̎��_
		&cameraWk.at,		// �J�����̒����_
		&cameraWk.up);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &cameraWk.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cameraWk.mtxProj);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&cameraWk.mtxProj,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &cameraWk.mtxProj);

}

//=============================================================================
//�J�����̏����擾����
//=============================================================================
CAMERA *GetCamera(void)
{
	return &cameraWk;
}

void DebugCamera()
{
	BeginDebugWindow("Camera");

	DebugText("Pos X:%f Y:%f Z:%f", cameraWk.pos.x, cameraWk.pos.y, cameraWk.pos.z);
	DebugText("At X:%f Y:%f Z:%f", cameraWk.at.x, cameraWk.at.y, cameraWk.at.z);
	DebugText("Rot X:%f Y:%f Z:%f", cameraWk.rot.x, cameraWk.rot.y, cameraWk.rot.z);

	EndDebugWindow("Camera");
}