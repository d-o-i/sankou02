//=============================================================================
//
// �J�[�\���\�� [Cursor.h]
// Author :  
//
//=============================================================================

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "_2dobj.h"
#include "CharacterAI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CURSOR_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)				// �T�C�Y
#define CURSOR_DIVIDE_X	(4)										// ������
#define CURSOR_DIVIDE_Y	(2)										// �c����
#define CURSOR_PATTERN	(CURSOR_DIVIDE_X * CURSOR_DIVIDE_Y)		// ������
#define CURSOR_FIRST_POS	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
#define CURSOR_SPEED	(30.0f)									// �����X�s�[�h

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Cursor : public _2dobj
{
private:
	int		ctrlNum;			// ���삷��R���g���[���ԍ�
	float	vec = 0.0f;			// �W���C�X�e�B�b�N�̃x�N�g����1/1000�ɂ��ĕۑ�(1.0f-0.0f)
	float	moveX = 0.0f;		// �W���C�X�e�B�b�N��X�l��1/1000�ɂ��ĕۑ�(1.0f-0.0f)
	float	moveY = 0.0f;		// �W���C�X�e�B�b�N��Y�l��1/1000�ɂ��ĕۑ�(1.0f-0.0f)
	//static LPDIRECT3DTEXTURE9 D3DTexture;					// �e�N�X�`���̃|�C���^

	// AI�p
	CharacterAI		*AIptr = nullptr;
	bool			AIUse = false;
	D3DXVECTOR3		WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	void PaintPath(void);					// �C���N�Œʂ�铹�����
	void DeletePath(void);					// ���̃v���C���[�̃y�C���g���폜
	void PaintObjChip(void);				// �I�u�W�F�N�g�`�b�v���y�C���g����

	HRESULT MakeVertex();				// ���_�̍쐬
	void SetTexture();					// �e�N�X�`�����W�̐ݒ�
	void SetVertex();					// ���_���W�̐ݒ�
	void KeyMove();						// �L�[�{�[�h����
	void PadMove();						// �R���g���[������
	void Move();						// ����
	void CalWorldPos();

public:
	Cursor(int PlayerNo, bool AIUse, CharacterAI *AIptr);
	~Cursor();

	// �X�V
	void Update();
	// �`��
	void Draw();	
	// �C���N��؂�ւ�
	void ChangeInk();						
	// �e�N�X�`���̊J��
	//static void ReleaseTexture(void) { SAFE_RELEASE(Cursor::D3DTexture); };

	// �J�[�\���̍��W���擾
	D3DXVECTOR3 GetPos(void) { return this->pos; };
	D3DXVECTOR3 GetWorldPos(void) { return this->WorldPos; };
};

#endif
