//=============================================================================
//
// その他関数まとめ [MyLibrary.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "MyLibrary.h"

//=============================================================================
// スクリーン座標をワールド座標に変換
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	int Sx,  // スクリーンX座標
	int Sy,  // スクリーンY座標
	float fZ,  // 射影空間でのZ値（0～1）
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Proj)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Proj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f;
	VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f;
	VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3((float)Sx, (float)Sy, (float)fZ), &tmp);

	return pout;
}

//=============================================================================
// ワールド座標からスクリーン座標に変換する
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
// 比較関数(降順)float用
// qsort関数の第4引数に入れて使う
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
// 比較関数(昇順)float用
// qsort関数の第4引数に入れて使う
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
// 比較関数(降順)int用
// qsort関数の第4引数に入れて使う
//=============================================================================
int CmpDescend(const void *p, const void *q)
{
	return *(int*)q - *(int*)p;
}

//=============================================================================
// 比較関数(昇順)int用
// qsort関数の第4引数に入れて使う
//=============================================================================
int CmpAscend(const void *p, const void *q)
{
	return *(int*)p - *(int*)q;
}

//=============================================================================
// CSVファイルの読み込み
// 引数：data = define定義したcsvデータ
// 引数：MapVector = ベクター
//=============================================================================
void ReadCsv(const char *data, vector<vector<int>> *MapVector)
{
	ifstream stream(data);		// マップの読み込み先
	string line;				// 文字列を一時的に保存
	const string delim = ",";	// データ区切り用の文字

	int row = 0;
	int col;
	while (getline(stream, line))
	{
		col = 0;
		// delimを区切り文字として切り分け、intに変換してmaptbl[][]に格納する
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
// カウントアップ関数
//=============================================================================
int LoopCountUp(int counter, int low, int high)
{
	counter++;
	return (counter > high) ? low : counter;
}

//=============================================================================
// カウントダウン関数
//=============================================================================
int LoopCountDown(int counter, int low, int high)
{
	counter--;
	return (counter < low) ? high : counter;
}

//=============================================================================
// floatのランダム値を取得（マイナス対応）
//=============================================================================
float RandomRange(float min, float max)
{
	return min + (float)(rand() % 1000) / 999.0f * (max - min);
}

//=============================================================================
// 球体表面上の任意座標を取得する
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
// 円周上の任意座標を取得する(X,Y,0.0f)
//=============================================================================
D3DXVECTOR3 GetCirclePosition(float Angle1, float Radius)
{
	D3DXVECTOR3 Pos;

	Pos.x = Radius * cosf(D3DXToRadian(Angle1));
	Pos.y = Radius * sinf(D3DXToRadian(Angle1));
	Pos.z = 0.0f;

	return Pos;

}