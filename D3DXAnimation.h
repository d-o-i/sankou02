//=============================================================================
//
// モデルのアニメーション処理 [D3DXAnimation.h]
// Author :  
//
//=============================================================================
#ifndef _D3DXANIMATION_H_
#define _D3DXANIMATION_H_

#include "AllocateHierarchy.h"
#include "AnimationSet.h"

#define Keydata_Max (15)

class D3DXANIMATION : public ID3DXAnimationCallbackHandler
{
private:
	// メンバ変数
	AllocateHierarchy			*AllocateHier;		// x fileの各情報を保存する
	LPD3DXFRAME					FrameRoot;			// ルートフレーム	
	UINT						CurrentAnimID;		// 現在再生しているアニメーションの番号
	UINT						PreventAnimID;		// 前再生していたアニメーションの番号

	D3DXFRAME_DERIVED* SearchBoneFrame(const char* BoneName, D3DXFRAME* Frame);
	void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void DrawFrame(LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

protected:

	vector<ANIMATIONSET>		AnimSet;
	LPD3DXANIMATIONCONTROLLER	AnimController;		// アニメーションコントローラー

	HRESULT Load_xFile(LPCTSTR filename, const char* ErrorSrc);
	HRESULT SetupCallbackKeys(vector<KEYDATA> *Keydata, LPCSTR SetName);
	void UpdateAnim(float Time);
	void DrawAnim(LPD3DXMATRIX WorldMatrix);

	D3DXMATRIX GetBoneMatrix(const char* BoneName);
	int GetAnimSetNum(void) { return this->AnimController->GetMaxNumAnimationSets(); };
	int GetAnimCurtFrame(void);
	int GetAnimPeriodFrame(void);
	LPCSTR GetCurtAnimName(void) { return this->AnimSet.at(this->CurrentAnimID).GetSetName(); };

	virtual void CreateAnimSet(void) = 0;
public:
	D3DXANIMATION();
	virtual ~D3DXANIMATION();
	// 純粋仮想関数
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void ChangeAnim(UINT AnimID);
	int GetAnimCurtID(void) { return this->CurrentAnimID; };

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


#endif
