//=============================================================================
//
// �C���N�Q�[�W�\������ [InkGauge.h]
// Author :  
//
//=============================================================================
#ifndef _InkGauge_H_
#define _InkGauge_H_

#include "_2dobj.h"

// �C���N�̎��
enum InkType
{
	ColorInk,
	BlackInk,
	InkNum,		// �C���N�̍ő吔
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class InkGauge : public _2dobj
{
private:
	int			InkType = 0;					// �g�p����C���N�̎��(0=color,1=black)
	int			Owner = 0;						// ���L��
	Vertex2D	GaugeVertex[NUM_VERTEX];		// ���_���i�[�\����
	Vertex2D	FrameVertex[NUM_VERTEX];		// ���_���i�[�\����

	LPDIRECT3DTEXTURE9 Texture_InkGauge;
	LPDIRECT3DTEXTURE9 Texture_GaugeFrame;

	HRESULT MakeVertex_GaugeFrame(int InkValue);
	HRESULT MakeVertex_InkGauge(int InkValue);
	void SetTexture_GaugeFrame(int InkValue);
	void SetTexture_InkGauge(int InkValue);
	void SetVertex_GaugeFrame(int InkValue);
	void SetVertex_InkGauge(int InkValue);

public:
	InkGauge(int InkType, int PlayerNo);
	~InkGauge();

	// �I�[�o�[���C�h�֐�
	void Update(int InkValue);
	void Draw();
};

#endif
