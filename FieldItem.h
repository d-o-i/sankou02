//=============================================================================
//
// �t�B�[���h��ɐ��������A�C�e�� [FieldItem.h]
// Author :  
//
//=============================================================================
#ifndef _FIELDITEM_H_
#define _FIELDITEM_H_

#include "Billboard.h"

#define FIELDITEM_SIZE	D3DXVECTOR2(50.0f, 50.0f)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class FieldItem :
	public Billboard
{
private:
	static LPDIRECT3DVERTEXBUFFER9	D3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	int								texno;			// �g�p����A�C�e���̃e�N�X�`���ԍ�
	bool							use;

public:
	FieldItem(int _texno, D3DXVECTOR3 _pos, D3DXVECTOR3 _move);
	~FieldItem();

	void Update()override;
	void Draw()override;

	HRESULT MakeVertex();
	void SetColor();

	void OnCamera();


	D3DXVECTOR3 GetPos() { return pos; };
	bool GetUse() { return use; };
	int GetTexNo() { return texno; };

	void SetUse(bool _use) { use = _use; };
};

#endif
