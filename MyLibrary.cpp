//=============================================================================
//
// ���̑��֐��܂Ƃ� [MyLibrary.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MyLibrary.h"

//=============================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	int Sx,  // �X�N���[��X���W
	int Sy,  // �X�N���[��Y���W
	float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Proj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Proj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f;
	VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f;
	VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3((float)Sx, (float)Sy, (float)fZ), &tmp);

	return pout;
}

//=============================================================================
// ���[���h���W����X�N���[�����W�ɕϊ�����
//=============================================================================
D3DXVECTOR2 WorldToScreenPos(D3DXMATRIX WorldMatrix)
{
	D3DXMATRIX ViewMatrix, ProjMatrix;
	D3DXMATRIX WVP;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	WVP = WorldMatrix * ViewMatrix * ProjMatrix;

	D3DXVECTOR3 ScreenCoord = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&ScreenCoord, &ScreenCoord, &WVP);

	ScreenCoord.x = ((ScreenCoord.x + 1.0f) / 2.0f) * SCREEN_WIDTH;
	ScreenCoord.y = ((-ScreenCoord.y + 1.0f) / 2.0f) * SCREEN_HEIGHT;

	return (D3DXVECTOR2)ScreenCoord;
}


//=============================================================================
// ��r�֐�(�~��)float�p
// qsort�֐��̑�4�����ɓ���Ďg��
//=============================================================================
int CmpDescendf(const void *p, const void *q)
{
	if (*(float*)p < *(float*)q)
	{
		return 1;
	}
	if (*(float*)p > *(float*)q)
	{
		return -1;
	}
	return 0;
}

//=============================================================================
// ��r�֐�(����)float�p
// qsort�֐��̑�4�����ɓ���Ďg��
//=============================================================================
int CmpAscendf(const void *p, const void *q)
{
	if (*(float*)p > *(float*)q)
	{
		return 1;
	}
	if (*(float*)p < *(float*)q)
	{
		return -1;
	}
	return 0;
}

//=============================================================================
// ��r�֐�(�~��)int�p
// qsort�֐��̑�4�����ɓ���Ďg��
//=============================================================================
int CmpDescend(const void *p, const void *q)
{
	return *(int*)q - *(int*)p;
}

//=============================================================================
// ��r�֐�(����)int�p
// qsort�֐��̑�4�����ɓ���Ďg��
//=============================================================================
int CmpAscend(const void *p, const void *q)
{
	return *(int*)p - *(int*)q;
}

//=============================================================================
// CSV�t�@�C���̓ǂݍ���
// �����Fdata = define��`����csv�f�[�^
// �����FMapVector = �x�N�^�[
//=============================================================================
void ReadCsv(const char *data, vector<vector<int>> *MapVector)
{
	ifstream stream(data);		// �}�b�v�̓ǂݍ��ݐ�
	string line;				// ��������ꎞ�I�ɕۑ�
	const string delim = ",";	// �f�[�^��؂�p�̕���

	int row = 0;
	int col;
	while (getline(stream, line))
	{
		col = 0;
		// delim����؂蕶���Ƃ��Đ؂蕪���Aint�ɕϊ�����maptbl[][]�Ɋi�[����
		for (string::size_type spos, epos = 0;
			(spos = line.find_first_not_of(delim, epos)) != string::npos;)
		{
			string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);
			MapVector->at(row).push_back(stoi(token));
		}
		++row;
	}
}

//=============================================================================
// �J�E���g�A�b�v�֐�
//=============================================================================
int LoopCountUp(int counter, int low, int high)
{
	counter++;
	return (counter > high) ? low : counter;
}

//=============================================================================
// �J�E���g�_�E���֐�
//=============================================================================
int LoopCountDown(int counter, int low, int high)
{
	counter--;
	return (counter < low) ? high : counter;
}

//=============================================================================
// float�̃����_���l���擾�i�}�C�i�X�Ή��j
//=============================================================================
float RandomRange(float min, float max)
{
	return min + (float)(rand() % 1000) / 999.0f * (max - min);
}

//=============================================================================
// ���̕\�ʏ�̔C�Ӎ��W���擾����
//=============================================================================
D3DXVECTOR3 GetSpherePosition(float Angle1, float Angle2, float Radius)
{
	D3DXVECTOR3 Pos;

	Pos.x = Radius * sinf(D3DXToRadian(Angle1)) * cosf(D3DXToRadian(Angle2));
	Pos.y = Radius * sinf(D3DXToRadian(Angle1)) * sinf(D3DXToRadian(Angle2));
	Pos.z = Radius * cosf(D3DXToRadian(Angle1));

	return Pos;
}

//=============================================================================
// �~����̔C�Ӎ��W���擾����(X,Y,0.0f)
//=============================================================================
D3DXVECTOR3 GetCirclePosition(float Angle1, float Radius)
{
	D3DXVECTOR3 Pos;

	Pos.x = Radius * cosf(D3DXToRadian(Angle1));
	Pos.y = Radius * sinf(D3DXToRadian(Angle1));
	Pos.z = 0.0f;

	return Pos;

}