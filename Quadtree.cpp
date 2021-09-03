//=============================================================================
//
// 四分木クラス[Quadtree.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Quadtree.h"
#include "Input.h"
#include "DebugWindow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// レベルを増やすなら、NodeIDの桁数注意
#define MAX_LEVEL (9)
#define MAX_OBJECTS (20)

bool RemoveCondition(Paint *Object) { return !Object->GetUse(); }

//=====================================================================================================
// コンストラクタ
//=====================================================================================================
QUADTREE::QUADTREE()
{
}

QUADTREE::QUADTREE(float x, float y, float Width, float Height, int Level, int NodeID)
{
	this->x = x;
	this->y = y;
	this->Width = Width;
	this->Height = Height;
	this->NodeID = NodeID;
	this->Level = Level;
	this->IsLeaf = true;
	this->LeftUpNode = nullptr;
	this->RightUpNode = nullptr;
	this->LeftDownNode = nullptr;
	this->RightDownNode = nullptr;
	this->Objects.reserve(MAX_OBJECTS);
}

//=====================================================================================================
// デストラクタ
//=====================================================================================================
QUADTREE::~QUADTREE()
{
	this->Clear();
}

//=====================================================================================================
// 更新処理
//=====================================================================================================
void QUADTREE::Update()
{
	if (this->RightUpNode)
	{
		this->RightUpNode->Update();
	}
	if (this->LeftUpNode)
	{
		this->LeftUpNode->Update();
	}
	if (this->LeftDownNode)
	{
		this->LeftDownNode->Update();
	}
	if (this->RightDownNode)
	{
		this->RightDownNode->Update();
	}

	CheckLeaves();

	this->Objects.erase(std::remove_if(std::begin(this->Objects), std::end(this->Objects), RemoveCondition), std::end(this->Objects));

#if _DEBUG
	if (GetKeyboardTrigger(DIK_L))
	{
		this->Objects.clear();
	}
#endif
}

//=====================================================================================================
// オブジェクトを入れる
//=====================================================================================================
void QUADTREE::InsertObject(Paint *Object)
{
	if (this->IsLeaf)
	{
		this->Objects.push_back(Object);
		Object->SetNodeID(this->NodeID);
		if (this->Objects.size() > MAX_OBJECTS && this->Level < MAX_LEVEL)
		{
			CreateLeaves();
			MoveObjectsToLeaves();
			this->IsLeaf = false;
		}
		return;
	}

	if (this->RightUpNode->IsContain(Object))
	{
		this->RightUpNode->InsertObject(Object);
		return;
	}
	else if (this->LeftUpNode->IsContain(Object))
	{
		this->LeftUpNode->InsertObject(Object);
		return;
	}
	else if (this->LeftDownNode->IsContain(Object))
	{
		this->LeftDownNode->InsertObject(Object);
		return;
	}
	else if (this->RightDownNode->IsContain(Object))
	{
		this->RightDownNode->InsertObject(Object);
		return;
	}

	this->Objects.push_back(Object);
	Object->SetNodeID(this->NodeID);
}

//=====================================================================================================
// 座標から同じツリーのオブジェクトを取得
//=====================================================================================================
//std::vector<Paint*> QUADTREE::GetObjectsAt(int NodeID, int Owner)
//{
//	std::vector<Paint*> ReserveObjects;
//	int NextNode = 0;
//
//	if (this->NodeID != NodeID)
//	{
//		if (this->IsLeaf)
//		{
//			for (auto &Paint : this->Objects)
//			{
//				if (Paint->GetOwner() != Owner && Paint->GetPaintColor() != BlackInkColor)
//				{
//					ReserveObjects.push_back(Paint);
//					return ReserveObjects;
//				}
//			}
//		}
//
//		NextNode = FindNode(NodeID);
//		if (NextNode == RightUpQuad)
//		{
//			ReserveObjects = this->RightUpNode->GetObjectsAt(NodeID, Owner);
//			return ReserveObjects;
//		}
//		else if (NextNode == LeftUpQuad)
//		{
//			ReserveObjects = this->LeftUpNode->GetObjectsAt(NodeID, Owner);
//			return ReserveObjects;
//		}
//		else if (NextNode == LeftDownQuad)
//		{
//			ReserveObjects = this->LeftDownNode->GetObjectsAt(NodeID, Owner);
//			return ReserveObjects;
//		}
//		else if (NextNode == RightDownQuad)
//		{
//			ReserveObjects = this->RightDownNode->GetObjectsAt(NodeID, Owner);
//			return ReserveObjects;
//		}
//	}
//
//	for (auto &Paint : this->Objects)
//	{
//		if (Paint->GetOwner() != Owner && Paint->GetPaintColor() != BlackInkColor)
//		{
//			ReserveObjects.push_back(Paint);
//		}
//	}
//
//	return ReserveObjects;
//}

std::vector<Paint*> QUADTREE::GetObjectsAt(int NodeID)
{
	std::vector<Paint*> ReserveObjects;
	ReserveObjects.reserve(30);
	int NextNode = 0;

	if (this->NodeID != NodeID)
	{
		if (this->IsLeaf)
		{
			ReserveObjects.insert(ReserveObjects.end(), this->Objects.begin(), this->Objects.end());
			return ReserveObjects;
		}

		NextNode = FindNode(NodeID);
		if (NextNode == RightUpQuad)
		{
			ReserveObjects = this->RightUpNode->GetObjectsAt(NodeID);
			return ReserveObjects;
		}
		else if (NextNode == LeftUpQuad)
		{
			ReserveObjects = this->LeftUpNode->GetObjectsAt(NodeID);
			return ReserveObjects;
		}
		else if (NextNode == LeftDownQuad)
		{
			ReserveObjects = this->LeftDownNode->GetObjectsAt(NodeID);
			return ReserveObjects;
		}
		else if (NextNode == RightDownQuad)
		{
			ReserveObjects = this->RightDownNode->GetObjectsAt(NodeID);
			return ReserveObjects;
		}
	}
	else
	{
		GetNodeObject(&ReserveObjects);
	}

	return ReserveObjects;
}

//=====================================================================================================
// このノード以下の全オブジェクトを取得
//=====================================================================================================
void QUADTREE::GetNodeObject(std::vector<Paint*> *VectorPtr)
{
	VectorPtr->insert(VectorPtr->end(), this->Objects.begin(), this->Objects.end());

	// 子供ノードのオブジェクトを探す
	if (this->RightUpNode)
	{
		this->RightUpNode->GetNodeObject(VectorPtr);
	}
	if (this->LeftUpNode)
	{
		this->LeftUpNode->GetNodeObject(VectorPtr);
	}
	if (this->LeftDownNode)
	{
		this->LeftDownNode->GetNodeObject(VectorPtr);
	}
	if (this->RightDownNode)
	{
		this->RightDownNode->GetNodeObject(VectorPtr);
	}
}

//=====================================================================================================
// 全てのオブジェクトをクリア
//=====================================================================================================
void QUADTREE::Clear(void)
{
	this->Objects.clear();
	ReleaseVector(Objects);

	if (!IsLeaf)
	{
		this->RightUpNode->Clear();
		this->LeftUpNode->Clear();
		this->LeftDownNode->Clear();
		this->RightDownNode->Clear();

		IsLeaf = true;
	}
}

//=====================================================================================================
// オブジェクトはこのツリーの中を確認
//=====================================================================================================
bool QUADTREE::IsContain(Paint *Object) const
{
	D3DXVECTOR2 Pos = Object->GetScreenPos();

	if (Pos.x >= this->x
		&& Pos.x <= this->x + this->Width
		&& Pos.y >= this->y
		&& Pos.y <= this->y + this->Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================================================================
// 容量が足りない、新しいツリー(葉っぱ)を分割
//=====================================================================================================
void QUADTREE::CreateLeaves(void)
{
	if (!this->RightUpNode)
	{
		this->RightUpNode = new QUADTREE(x + Width / 2, y, Width / 2, Height / 2, Level + 1, CalChildNodeID(RightUpQuad));
	}
	if (!this->LeftUpNode)
	{
		this->LeftUpNode = new QUADTREE(x, y, Width / 2, Height / 2, Level + 1, CalChildNodeID(LeftUpQuad));
	}
	if (!this->LeftDownNode)
	{
		this->LeftDownNode = new QUADTREE(x, y + Height / 2, Width / 2, Height / 2, Level + 1, CalChildNodeID(LeftDownQuad));
	}
	if (!this->RightDownNode)
	{
		this->RightDownNode = new QUADTREE(x + Width / 2, y + Height / 2, Width / 2, Height / 2, Level + 1, CalChildNodeID(RightDownQuad));
	}
}

//=====================================================================================================
// 子供ツリーを削除
//=====================================================================================================
void QUADTREE::DestroyLeaves(void)
{
	this->RightUpNode->Clear();
	SAFE_DELETE(this->RightUpNode);
	this->LeftUpNode->Clear();
	SAFE_DELETE(this->LeftUpNode);
	this->LeftDownNode->Clear();
	SAFE_DELETE(this->LeftDownNode);
	this->RightDownNode->Clear();
	SAFE_DELETE(this->RightDownNode);
}

//=====================================================================================================
// 子供ツリーを検査
//=====================================================================================================
void QUADTREE::CheckLeaves(void)
{
	// ノードが葉っぱ、子供ノードがない
	if (this->IsLeaf)
	{
		return;
	}
	else
	{
		// 子供ノードが存在すれば
		if (this->RightUpNode)
		{
			this->RightUpNode->CheckLeaves();
		}
		if (this->LeftUpNode)
		{
			this->LeftUpNode->CheckLeaves();
		}
		if (this->LeftDownNode)
		{
			this->LeftDownNode->CheckLeaves();
		}
		if (this->RightDownNode)
		{
			this->RightDownNode->CheckLeaves();
		}

		// このノード以下のオブジェクト数は納められる容量より小さい、ツリーを整理する
		if (this->GetNodeObjectNum() <= MAX_OBJECTS)
		{
			// 子供ノードのオブジェクトが親に移動
			MoveObjectsToNode();
			// 子供ノードを削除
			DestroyLeaves();
			this->IsLeaf = true;
		}
	}
}

//=====================================================================================================
// 親のオブジェクトが子供ツリーに移動
//=====================================================================================================
void QUADTREE::MoveObjectsToLeaves(void)
{
	for (auto Paint = Objects.begin(); Paint != Objects.end();)
	{
		if (this->RightUpNode->IsContain(*Paint))
		{
			this->RightUpNode->InsertObject(*Paint);
			Paint = Objects.erase(Paint);
		}
		else if (this->LeftUpNode->IsContain(*Paint))
		{
			this->LeftUpNode->InsertObject(*Paint);
			Paint = Objects.erase(Paint);
		}
		else if (this->LeftDownNode->IsContain(*Paint))
		{
			this->LeftDownNode->InsertObject(*Paint);
			Paint = Objects.erase(Paint);
		}
		else if (this->RightDownNode->IsContain(*Paint))
		{
			this->RightDownNode->InsertObject(*Paint);
			Paint = Objects.erase(Paint);
		}
		else
		{
			Paint++;
		}
	}
}

//=====================================================================================================
// 子供のオブジェクトが親ツリーに移動
//=====================================================================================================
void QUADTREE::MoveObjectsToNode(void)
{
	if (this->RightUpNode)
	{
		this->Objects.insert(this->Objects.end(), this->RightUpNode->Objects.begin(), this->RightUpNode->Objects.end());
	}
	if (this->LeftUpNode)
	{
		this->Objects.insert(this->Objects.end(), this->LeftUpNode->Objects.begin(), this->LeftUpNode->Objects.end());
	}
	if (this->LeftDownNode)
	{
		this->Objects.insert(this->Objects.end(), this->LeftDownNode->Objects.begin(), this->LeftDownNode->Objects.end());
	}
	if (this->RightDownNode)
	{
		this->Objects.insert(this->Objects.end(), this->RightDownNode->Objects.begin(), this->RightDownNode->Objects.end());
	}

	for (auto &Object : this->Objects)
	{
		Object->SetNodeID(this->NodeID);
	}
}

//=====================================================================================================
// このノード以下の全オブジェクトの数を取得
//=====================================================================================================
int QUADTREE::GetNodeObjectNum(void)
{
	int ObjectNum = 0;

	// 子供ノードのオブジェクトの数を探す
	if (this->RightUpNode)
	{
		ObjectNum += this->RightUpNode->GetNodeObjectNum();
	}
	if (this->LeftUpNode)
	{
		ObjectNum += this->LeftUpNode->GetNodeObjectNum();
	}
	if (this->LeftDownNode)
	{
		ObjectNum += this->LeftDownNode->GetNodeObjectNum();
	}
	if (this->RightDownNode)
	{
		ObjectNum += this->RightDownNode->GetNodeObjectNum();
	}

	ObjectNum += this->Objects.size();

	return ObjectNum;
}

//=====================================================================================================
// 子供ツリーのノード番号を計算
//=====================================================================================================
int QUADTREE::CalChildNodeID(e_QuadType QuadType)
{
	return (this->NodeID * 10 + QuadType);
}

//=====================================================================================================
// ノード番号で所属するツリーを検査
//=====================================================================================================
int QUADTREE::FindNode(int NodeID)
{
	char NodeID_char[MAX_LEVEL + 1] = { NULL };
	int ChildNodeID = 0;

	sprintf_s(NodeID_char, "%d", NodeID);
	ChildNodeID = NodeID_char[this->Level] - '0';

	return ChildNodeID;
}

//=====================================================================================================
// オブジェクトがノードの範囲内かどうかを調べる
//=====================================================================================================
bool QUADTREE::CheckObjectInNode(Paint *Object)
{
	int NextNode = 0;
	int ObjNodeID = Object->GetNodeID();

	if (this->NodeID != ObjNodeID)
	{
		NextNode = FindNode(ObjNodeID);
		if (NextNode == RightUpQuad)
		{
			return this->RightUpNode->CheckObjectInNode(Object);
		}
		else if (NextNode == LeftUpQuad)
		{
			return this->LeftUpNode->CheckObjectInNode(Object);
		}
		else if (NextNode == LeftDownQuad)
		{
			return this->LeftDownNode->CheckObjectInNode(Object);
		}
		else if (NextNode == RightDownQuad)
		{
			return this->RightDownNode->CheckObjectInNode(Object);
		}
	}
	else
	{
		if (!IsContain(Object))
		{
			for (auto i = this->Objects.begin(); i != this->Objects.end(); i++)
			{
				if ((*i) == Object)
				{
					i = this->Objects.erase(i);
					break;
				}
			}
			return false;
		}
	}
	return true;
}
