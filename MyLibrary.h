//=============================================================================
//
// ���̑��֐��܂Ƃ� [MyLibrary.h]
// Author :  
//
//=============================================================================
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, 
	int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Proj);	// �X�N���[�����W�����[���h���W�֕ϊ�
D3DXVECTOR2 WorldToScreenPos(D3DXMATRIX WorldMatrix);

int CmpDescendf(const void *p, const void *q);		// float�̔�r�i�~���j
int CmpAscendf(const void *p, const void *q);		// float�̔�r�i�����j
int CmpDescend(const void *p, const void *q);		// int�̔�r�i�~���j
int CmpAscend(const void *p, const void *q);		// int�̔�r�i�����j

template<typename T> 
T clamp(T x, T low, T high)							// ����A�����̃`�F�b�N
{
	return min(max(x, low), high);
}

void ReadCsv(const char *data, vector<vector<int>> *MapVector);	// csv�f�[�^�̓ǂݍ���
int LoopCountUp(int counter, int low, int high);				// �J�E���g�A�b�v�֐�
int LoopCountDown(int counter, int low, int high);				// �J�E���g�_�E���֐�

float RandomRange(float min, float max);						// float�̃����_���l���擾�i�}�C�i�X�Ή��j

D3DXVECTOR3 GetSpherePosition(float Angle1, float Angle2, float Radius);	// ���̕\�ʏ�̔C�Ӎ��W���擾����
D3DXVECTOR3 GetCirclePosition(float Angle1, float Radius);	// �~����̔C�Ӎ��W���擾����(X,Y,0.0f)

#endif

