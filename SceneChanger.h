//=============================================================================
//
// シーン切り替え [SceneChanger.h]
// Author :  
// 
//=============================================================================
#ifndef _SCENECHANGER_H_
#define _SCENECHANGER_H_

//*****************************************************************************
// クラス定義
//*****************************************************************************
class SceneChanger
{
public:
	SceneChanger();
	virtual ~SceneChanger();

	virtual void Update() = 0;
	virtual void DrawMask();
	virtual void DrawChanger();
	virtual void SetChanger(bool isStart, std::function<void(void)> func = NULL) = 0;

	// 使用するテクスチャの取得
	void LoadMaskTexture();
	void LoadChangeTexture();

	bool GetUseMask() { return useMask; };

protected:
	bool useMask;
	bool useChanger;
	bool isChangerOut;

	LPDIRECT3DTEXTURE9 maskTexture, changeTexture;
	Vertex2D vertexWkMask[NUM_VERTEX], vertexWkChange[NUM_VERTEX];
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;

	float radius, angle;

	void SetVertexMask();
	std::function<void(void)> callback;

	virtual void BeginMask();
	virtual void EndMask();
	
	virtual void MakeVertexMask();
	virtual void MakeVertexChanger();
};

#endif
