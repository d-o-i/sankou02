//=============================================================================
//
// ���C�g���� [Light.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Light.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9			Light[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[0], sizeof(D3DLIGHT9));

	// ���C�g0�̃^�C�v�̐ݒ�
	Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g0�̊g�U���̐ݒ�
	Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g0�̊����̐ݒ�
	Light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g0�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[0].Direction, &vecDir);

	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &Light[0]);

	// ���C�g0���g�p�g�p��Ԃ�
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[1], sizeof(D3DLIGHT9));

	// ���C�g1�̃^�C�v�̐ݒ�
	Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g1�̊g�U���̐ݒ�
	Light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ���C�g1�̊����̐ݒ�
	Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g1�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[1].Direction, &vecDir);

	// ���C�g1�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &Light[1]);

	// ���C�g1�̐ݒ�
	pDevice->LightEnable(1, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[2], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// ����
	Light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(2, &Light[2]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(2, TRUE);


	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// ���C�g�̃Q�b�^�[
//=============================================================================
D3DLIGHT9 *GetLight(int no)
{
	return &Light[no];
}