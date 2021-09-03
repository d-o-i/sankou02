#ifndef _FIELDITEMMANAGER_H_
#define _FIELDITEMMANAGER_H_

#include "FieldItem.h"

class FieldItemManager
{
private:
	std::vector<FieldItem*> Item;

public:
	FieldItemManager();
	~FieldItemManager();

	void Update();
	void Draw();

	void Check();
	void Set(int ItemID, D3DXVECTOR3 pos, D3DXVECTOR3 move);

	std::vector<FieldItem*> GetItem() { return Item; };
};

#endif
