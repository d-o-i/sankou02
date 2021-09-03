//=============================================================================
//
// �o�g����ʃt���[���\������ [Carslobj.h]
// Author : 
//
//=============================================================================
#ifndef _CARSLOBJ_H_
#define _CARSLOBJ_H_

#include "_2dobj.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_CARSLOBJ	_T("data/texture/charasl_obj.png")			// �t���[���p�摜
#define TEXTURE_CARSLOBJ2	_T("data/texture/charasl_obj2.png")			// �t���[���p�摜
#define TEXTURE_CARSLOBJ3	_T("data/texture/charasl_obj3.png")			// �t���[���p�摜
#define TEXTURE_CARSLOBJ4	_T("data/texture/charasl_obj4.png")			// �t���[���p�摜
#define CARSLOBJ_SIZE		D3DXVECTOR3(114.0f, 114.0f, 0.0f)		// �e�N�X�`���T�C�Y
#define AO					(0)
#define AKA					(1)
#define MIDORI				(2)
#define KI					(3)
#define CARSLOBJ_POS01		D3DXVECTOR3(400.0f, 300.0f, 0.0f)
#define CARSLOBJ_POS02		D3DXVECTOR3(550.0f, 300.0f, 0.0f)
#define CARSLOBJ_POS03		D3DXVECTOR3(700.0f, 300.0f, 0.0f)
// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W

} VERTEX_2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Carslobj :
	public _2dobj {
public:
	bool  player = false;

	Carslobj(D3DXVECTOR3 _pos, const char *texno);
	~Carslobj();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	HRESULT MakeVertex();
	void SetTexture(int cntPattern);
	void SetVertex();

};
int Getpnum();
void Setpnum(int no);
bool Getch();
void Setch(bool ch);

#endif