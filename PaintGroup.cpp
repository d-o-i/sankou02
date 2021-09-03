//=============================================================================
//
// ペイントグループクラス[PaintGroup.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Paint.h"
#include "PaintGroup.h"
#include "Input.h"
#include "DebugWindow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GroupCapacity (10)
#define PosCapacity (15)

bool RemoveCondition(GroupStruct Group) { return Group.Count <= 0 ? true : false; }

//=====================================================================================================
// コンストラクタ
//=====================================================================================================
PaintGroup::PaintGroup()
{
	GroupVector.reserve(GroupCapacity);
}


//=====================================================================================================
// デストラクタ
//=====================================================================================================
PaintGroup::~PaintGroup()
{
	GroupVector.clear();
	ReleaseVector(GroupVector);
}

//=====================================================================================================
// 更新処理
//=====================================================================================================
void PaintGroup::Update()
{
	for (auto &Group : GroupVector)
	{
		if (Group.Use == true && Group.PaintOver == true)
		{
			Group.Count--;
		}
	}

	GroupVector.erase(std::remove_if(std::begin(GroupVector), std::end(GroupVector), RemoveCondition), std::end(GroupVector));

#if _DEBUG
	if (GetKeyboardTrigger(DIK_L))
	{
		GroupVector.clear();
	}

	ImGui::SetNextWindowPos(ImVec2(1050, 120), ImGuiSetCond_Once);

	BeginDebugWindow("PaintGroup");

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);

	if (!GroupVector.empty())
	{
		int i = 0;
		for (auto &Group : GroupVector)
		{
			i++;
			if (ImGui::TreeNode((void*)(intptr_t)i, "PaintGroup %d", i))
			{
				if (ImGui::TreeNode("Position"))
				{
					for (auto &Pos : Group.PaintPath)
					{
						DebugText("X : %.2f , Y : %.2f\n", Pos.x, Pos.y);
					}
					ImGui::TreePop();
				}


				if (Group.Use)
				{
					DebugText("Use : true\n");
				}
				else
				{
					DebugText("Use : false\n");
				}

				if (Group.PaintOver)
				{
					DebugText("PaintOver : true\n");
				}
				else
				{
					DebugText("PaintOver : false\n");
				}

				DebugText("Count : %d\n", Group.Count);

				ImGui::TreePop();
			}
		}
	}

	EndDebugWindow("PaintGroup");

#endif
}

bool PaintGroup::GetEnemyPaint(GroupStruct** GroupPtr, int PlayerNo)
{
	std::vector<GroupStruct*> TempVec;

	for (auto &Group : GroupVector)
	{
		if (Group.Use == true && Group.PaintOver == true && Group.Onwer != PlayerNo)
		{
			TempVec.push_back(&Group);
		}
	}

	if (TempVec.empty())
	{
		GroupPtr = nullptr;
		return false;
	}
	else
	{
		if (TempVec.size() > 1)
		{
			int Random = rand() % TempVec.size();
			*GroupPtr = TempVec.at(Random);
			(*GroupPtr)->Use = false;
		}
		else if (TempVec.size() == 1)
		{
			*GroupPtr = TempVec.at(0);
			(*GroupPtr)->Use = false;
		}
		return true;
	}
}

void PaintGroup::Start(int Owner)
{
	std::vector<D3DXVECTOR3> PaintPath;
	PaintPath.reserve(PosCapacity);

	GroupVector.push_back(GroupStruct{ PaintPath, Owner, DRAW_FRAME_COLOR, true, false });
}

void PaintGroup::Insert(D3DXVECTOR3 Pos, int Owner)
{
	for (auto &Group : GroupVector)
	{
		if (Group.Onwer == Owner && Group.PaintOver == false)
		{
			Group.PaintPath.push_back(Pos);
			break;
		}
	}
}

void PaintGroup::End(int Owner)
{
	for (auto &Group : GroupVector)
	{
		if (Group.Onwer == Owner && Group.PaintOver == false)
		{
			Group.PaintOver = true;
			break;
		}
	}
}
