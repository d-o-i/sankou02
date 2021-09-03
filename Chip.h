//=============================================================================
//
// �`�b�v���� [Chip.h]
// Author :  
//
//=============================================================================
#ifndef _CHIP_H_
#define _CHIP_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHIP_SIZE		(20.0f)								// �}�b�v�`�b�v�ꖇ�̃T�C�Y


enum eChipType
{
	eMapChip,
	eObjectChip,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Chip
{
private:
	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	D3DXVECTOR3				scl;
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		D3DTexture;			// �e�N�X�`���ւ̃|�C���^
	bool use;
	int ChipType;
	bool reverse;
	int cnt;
	int texnum;
	int mapX, mapY;

	HRESULT MakeVertex();
	void CheckOnCamera();
	void SetTexture();

public:
	Chip(int x, int y, int _texnum, int ChipType);
	~Chip();

	void Update();
	void Draw();

	void ReverseTexture();

	D3DXVECTOR3 GetPos() { return pos; };
	bool GetUse() { return use; };
	bool GetReverse() { return reverse; };
	int GetTextureNum() { return texnum; };

	void SetReverse(bool _reverse) { reverse = _reverse; };
	void SetCnt(int num) { cnt = num; };
};

#endif
