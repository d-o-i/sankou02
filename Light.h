//=============================================================================
//
// ���C�g���� [light.h]
// Author :  
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_LIGHT		(3)												// ���C�g�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLight(void);
D3DLIGHT9 *GetLight(int no);

#endif