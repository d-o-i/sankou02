//=============================================================================
//
// �y�C���g [Paint.h]
// Author :  
//
//=============================================================================
#ifndef _PAINT_H_
#define _PAINT_H_

#include "Billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PAINT_WIDTH			(30.0f)						// ��
#define	PAINT_HEIGHT		(30.0f)						// ����
#define PAINT_DIVIDE_X		(5)
#define PAINT_DIVIDE_Y		(1)
#define PAINT_DIVIDE		(PAINT_DIVIDE_X * PAINT_DIVIDE_Y)
#define DRAW_FRAME_COLOR	(300)						// �\�����Ă����t���[����
#define DRAW_FRAME_BLACK	(60)						// �\�����Ă����t���[����

enum InkColor
{
	RedInkColor,
	BlueInkColor,
	YellowInkColor,
	GreenInkColor,
	BlackInkColor
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Paint : public Billboard
{
private:
	bool					Use;			// �g�p���Ă��邩�ǂ���
	bool					InScreen;		// ��ʓ��t���O
	int						time;			// �\�����鎞��
	int						NodeID;			// �l���ؗp�m�[�hID
	int						Owner;			// ���L��
	int						PaintColor;		// �y�C���g�J���[
	D3DXVECTOR2				ScreenPos;		// �X�N���[�����W

	static float			HalfSize;		// �y�C���g�T�C�Y�̔���

	void SetColor();
	HRESULT MakeVertex();
	// ���[���h���W����X�N���[�����W�ɕϊ�����
	void CheckInScreen(D3DXMATRIX WorldMatrix);

public:
	Paint(int Owner, int PaintColor);
	~Paint();

	void Update();
	void Draw();

	// �Q�b�^�[
	bool GetUse() { return this->Use; };
	bool GetInScreen() { return this->InScreen; };
	D3DXVECTOR3 GetPos() { return pos; };
	int GetNodeID(void) { return this->NodeID; };
	int GetOwner(void) { return this->Owner; };
	int GetPaintColor(void) { return this->PaintColor; };
	D3DXVECTOR2 GetScreenPos(void) { return this->ScreenPos; };
	static float GetPaintRadius(void) { return Paint::HalfSize; };

	// �Z�b�^�[
	void SetPos(D3DXVECTOR3 _pos) { pos = _pos; };
	void SetUse(bool Flag) { this->Use = Flag; };
	void SetTime(int _time) { time = _time; };
	void SetNodeID(int NodeID) { this->NodeID = NodeID; };
	void SetScreenPos(D3DXVECTOR2 ScreenPos) { this->ScreenPos = ScreenPos; };
};

#endif
