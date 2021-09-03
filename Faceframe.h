//=============================================================================
//
// �o�g����ʃt���[���\������ [Faceframe.h]
// Author : 
//
//=============================================================================
#ifndef _FACEFRAME_H_
#define _FACEFRAME_H_

#include "_2dobj.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_FACEFRAME	_T("data/texture/faceframe.png")			// �t���[���p�摜
#define TEXTURE_FACE1		_T("data/texture/charasl_obj.png")			// �t���[���p�摜
#define TEXTURE_FACE2		_T("data/texture/charasl_obj2.png")			// �t���[���p�摜
#define TEXTURE_FACE3		_T("data/texture/charasl_obj3.png")			// �t���[���p�摜
#define TEXTURE_FACE4		_T("data/texture/charasl_obj4.png")			// �t���[���p�摜

#define FACEFRAME_POS01     D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define FACEFRAME_POS02		D3DXVECTOR3(318.0f, 0.0f, 0.0f)
#define FACEFRAME_POS03		D3DXVECTOR3(636.0f, 0.0f, 0.0f)
#define FACEFRAME_POS04		D3DXVECTOR3(954.0f, 0.0f, 0.0f)

//#define FACE1_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
//#define FACE2_POS			D3DXVECTOR3(318.0f, 0.0f, 0.0f)
//#define FACE3_POS			D3DXVECTOR3(636.0f, 0.0f, 0.0f)
//#define FACE4_POS			D3DXVECTOR3(954.0f, 0.0f, 0.0f)

#define FACEFRAME1_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define FACEFRAME2_POS			D3DXVECTOR3(318.0f, 0.0f, 0.0f)
#define FACEFRAME3_POS			D3DXVECTOR3(636.0f, 0.0f, 0.0f)
#define FACEFRAME4_POS			D3DXVECTOR3(954.0f, 0.0f, 0.0f)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class FaceFrame : public _2dobj 
{
private:
	HRESULT MakeVertex(void);
	void SetVertex(void);

public:
	FaceFrame(int PlayerNo);
	~FaceFrame();

	// �I�[�o�[���C�h�֐�
	void Update();
	void Draw();
	void SetPlayerDeadTexture(void);
};

#endif