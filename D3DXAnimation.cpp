//=============================================================================
//
// モデルのアニメーション処理 [D3DXAnimation.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "D3DXAnimation.h"
#include "AnimationSet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// コンストラクタ
//=============================================================================
D3DXANIMATION::D3DXANIMATION()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
D3DXANIMATION::~D3DXANIMATION()
{
	SAFE_RELEASE(this->AnimController);

	D3DXFrameDestroy(this->FrameRoot, this->AllocateHier);

	this->AllocateHier->DestroyFrame(this->FrameRoot);

	this->AnimSet.clear();
	ReleaseVector(this->AnimSet);

	SAFE_DELETE(this->AllocateHier);
}

//=============================================================================
// Xファイルの読み込み
//=============================================================================
HRESULT D3DXANIMATION::Load_xFile(LPCTSTR filename, const char* ErrorSrc)
{
	char Message[64];
	LPDIRECT3DDEVICE9 Device = GetDevice();
	this->AllocateHier = new AllocateHierarchy();

	if (
		FAILED(D3DXLoadMeshHierarchyFromX(filename,
			D3DXMESH_MANAGED,
			Device,
			this->AllocateHier,
			NULL,
			&this->FrameRoot,
			&this->AnimController))
		)
	{
		sprintf_s(Message, "Load %s Model Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	if (FAILED(SetupBoneMatrixPointers(this->FrameRoot, this->FrameRoot)))
	{
		return E_FAIL;
	}

	this->AnimSet.reserve(this->GetAnimSetNum());

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void D3DXANIMATION::UpdateAnim(float Time)
{
	float CurWeightTime = this->AnimSet.at(this->CurrentAnimID).GetCurWeightTime();
	float ShiftTime = this->AnimSet.at(this->CurrentAnimID).GetShiftTime();

	// 合成中か否かを判定
	CurWeightTime += Time;
	this->AnimSet.at(this->CurrentAnimID).SetCurWeightTime(CurWeightTime);

	if (CurWeightTime <= ShiftTime)
	{
		// 合成中。ウェイトを算出
		float Weight = CurWeightTime / ShiftTime;
		// ウェイトを登録
		this->AnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		this->AnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		this->AnimController->SetTrackWeight(0, 1.0f);			// 現在のアニメーション
		this->AnimController->SetTrackEnable(1, false);		// 前のアニメーションを無効にする
	}

	// 時間を更新
	this->AnimController->AdvanceTime(Time, this);

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void D3DXANIMATION::DrawAnim(LPD3DXMATRIX WorldMatrix)
{
	UpdateFrameMatrices(this->FrameRoot, WorldMatrix);

	DrawFrame(this->FrameRoot);

	return;
}

//=============================================================================
// アニメーションを切り替え
//=============================================================================
void D3DXANIMATION::ChangeAnim(UINT AnimID)
{
	D3DXTRACK_DESC TD;   // トラックの能力
	ID3DXAnimationSet* CurtAnimSet = this->AnimSet.at(this->CurrentAnimID).GetAnimSetPtr();
	ID3DXAnimationSet* NextAnimSet = this->AnimSet.at(AnimID).GetAnimSetPtr();
	float Speed = this->AnimSet.at(AnimID).GetSpeed();

	// 指定のアニメーションIDの存在をチェック
	if (AnimID > (UINT)this->GetAnimSetNum())
	{
		MessageBox(0, "AnimationSet Don't Exist", "Error", 0);
		return;
	}

	// 異なるアニメーションであるかをチェック
	if (this->CurrentAnimID == AnimID)
	{
		// 更新する必要がない
		return;
	}

	// 現在のアニメーションセットの設定値を取得
	this->AnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	this->AnimController->SetTrackAnimationSet(1, CurtAnimSet);
	this->AnimController->SetTrackDesc(1, &TD);
	this->AnimController->SetTrackSpeed(1, 0.0f);

	// 新しいアニメーションセットをトラック0に設定
	this->AnimController->SetTrackAnimationSet(0, NextAnimSet);

	// トラック0のスピードの設定
	this->AnimController->SetTrackSpeed(0, Speed);

	// トラック0の位置は最初からに設定
	this->AnimController->SetTrackPosition(0, 0.0f);

	// トラックの合成を許可
	this->AnimController->SetTrackEnable(0, true);
	this->AnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	this->AnimSet.at(AnimID).SetCurWeightTime(0.0f);
	this->AnimController->ResetTime();

	// 現在のアニメーション番号を切り替え
	this->PreventAnimID = this->CurrentAnimID;
	this->CurrentAnimID = AnimID;

	return;
}

//=============================================================================
// 特定なボーンを探す
//=============================================================================
D3DXFRAME_DERIVED* D3DXANIMATION::SearchBoneFrame(const char* BoneName, D3DXFRAME* Frame)
{
	D3DXFRAME_DERIVED* pFrame = NULL;

	if (Frame == NULL)
	{
		return NULL;
	}

	if (Frame->Name != NULL && strcmp(Frame->Name, BoneName) == 0)
	{
		pFrame = (D3DXFRAME_DERIVED*)Frame;
		return pFrame;
	}

	if (Frame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(BoneName, Frame->pFrameSibling);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	if (Frame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(BoneName, Frame->pFrameFirstChild);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	return NULL;
}

//=============================================================================
// 特定のボーンマトリックスを取得
//=============================================================================
D3DXMATRIX D3DXANIMATION::GetBoneMatrix(const char* BoneName)
{
	char Message[64];
	D3DXFRAME_DERIVED* pFrame = SearchBoneFrame(BoneName, this->FrameRoot);

	// ボーンが見つかれば
	if (pFrame != NULL && pFrame->Name != NULL && strcmp(pFrame->Name, BoneName) == 0)
	{
		return pFrame->CombinedTransformationMatrix;
	}
	// ボーンが見つからなければ
	else
	{
		sprintf_s(Message, "Find %s Bone Failed！", BoneName);
		MessageBox(0, Message, "Error", 0);
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}

//=============================================================================
// アニメーションセットのCallbackKeysを設置する
//=============================================================================
HRESULT D3DXANIMATION::SetupCallbackKeys(vector<KEYDATA> *Keydata, LPCSTR SetName)
{
	// Keyframeを設置する予定のAnimationSet
	ID3DXKeyframedAnimationSet* AnimSetTemp = NULL;
	// Keyframeを設置した新しいAnimationSet
	ID3DXCompressedAnimationSet* CompressedAnimSet = NULL;
	// 圧縮された資料
	ID3DXBuffer* CompressedInfo = NULL;
	// エラーメッセージ	
	char Message[256] = { NULL };
	double Ticks = 0.0;
	double Period = 0.0;
	// 設置するCallback Keys
	D3DXKEY_CALLBACK *Keys = NULL;
	vector<KEYDATA> KeyVector = *Keydata;
	int CallbacksNum = KeyVector.size();

	if (CallbacksNum == 0)
	{
		goto FunctionExit;
	}

	if (FAILED(this->AnimController->GetAnimationSetByName(SetName, (ID3DXAnimationSet**)&AnimSetTemp)))
	{
		sprintf_s(Message, "Setup Callbacks in %s AnimationSet Failed！", SetName);
		goto FunctionExit;
	}

	// 圧縮資料を渡す
	AnimSetTemp->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &CompressedInfo);

	// 1秒のキーフレイムの数を返す(1秒 = 4800)
	Ticks = AnimSetTemp->GetSourceTicksPerSecond();
	// アニメーションのループ時間
	Period = AnimSetTemp->GetPeriod();

	// メモリを配る
	Keys = (D3DXKEY_CALLBACK*)calloc(CallbacksNum, sizeof(D3DXKEY_CALLBACK));
	if (Keys == NULL)
	{
		sprintf_s(Message, "Alloc Keys in %s AnimationSet Failed！", SetName);
		goto FunctionExit;
	}

	// Callback Keysを設置する
	for (int i = 0; i < CallbacksNum; i++)
	{
		if (KeyVector[i].KeyType == 0)
		{
			sprintf_s(Message, "There is NoEvent Keydata in %s AnimationSet！", SetName);
			goto FunctionExit;
		}
		Keys[i].Time = (float)(Period * Ticks * KeyVector[i].Time);
		Keys[i].pCallbackData = (void*)KeyVector[i].KeyType;
	}

	// 圧縮するアニメーションセットを作成する
	D3DXCreateCompressedAnimationSet(AnimSetTemp->GetName(),
		AnimSetTemp->GetSourceTicksPerSecond(),
		AnimSetTemp->GetPlaybackType(), CompressedInfo, CallbacksNum, Keys, &CompressedAnimSet);

	// 古いアニメーションセット(圧縮されない)を削除する
	this->AnimController->UnregisterAnimationSet(AnimSetTemp);

	// 新しいアニメーションセット(圧縮された)を追加する
	this->AnimController->RegisterAnimationSet(CompressedAnimSet);

	// 終了処理
FunctionExit:

	SAFE_RELEASE(CompressedInfo);
	SAFE_RELEASE(AnimSetTemp);
	SAFE_RELEASE(CompressedAnimSet);
	ReleaseVector(KeyVector);
	SAFE_FREE(Keys);

	if (Message[0] == NULL)
	{
		return S_OK;
	}
	else
	{
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}
}

//=============================================================================
// ボーン行列のポインタの準備
//=============================================================================
HRESULT D3DXANIMATION::SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot)
{
	if (pFrameBase->pMeshContainer != NULL)
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrix[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// フレームの描画
//=============================================================================
void D3DXANIMATION::DrawFrame(LPD3DXFRAME pFrame)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPD3DXMESHCONTAINER pMeshContainer;

	if (pFrame == NULL)
	{
		return;
	}
	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	DrawFrame(pFrame->pFrameSibling);
	DrawFrame(pFrame->pFrameFirstChild);
}


//=============================================================================
// メッシュコンテナの描画
//=============================================================================
void D3DXANIMATION::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	Device->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->BoneNum; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrix[iMatrixIndex]);
						Device->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				Device->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					Device->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					Device->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		Device->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
	else  // standard mesh, just draw it after setting material properties
	{
		Device->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			Device->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			Device->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

//=============================================================================
// フレーム行列の更新
//=============================================================================
void D3DXANIMATION::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (pFrameBase == NULL || pParentMatrix == NULL)
	{
		return;
	}

	D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);

	UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
}

int D3DXANIMATION::GetAnimCurtFrame(void)
{
	// 60 FPS
	double TrackTime = this->AnimController->GetTime() * 60;
	double SetPeriod = GetAnimPeriodFrame();
	return (int)(fmod(TrackTime, SetPeriod));
}

int D3DXANIMATION::GetAnimPeriodFrame(void)
{
	return (int)(this->AnimSet.at(this->CurrentAnimID).GetPeriod() * 60);
}
