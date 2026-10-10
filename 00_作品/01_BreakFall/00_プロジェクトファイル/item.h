//========================================================================================================
//
// アイテムの処理[item.h]
// Author ItoKotaro
//
//========================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "behavior.h"

//アイテム（基底）
class Item : public Behavior		//（親）挙動
{
public:
	Item(Object* pObject) : Behavior(pObject) {}
	void Init() override;
	void Update() override;
	void Collision(Object* pObject, Physics::CONTACT contact) override;

	static const float SIZE;
	static const float FALL_SPEED;
private:
	virtual void onHit() = 0;
};

//玉増やす
class AddBallItem : public Item
{
public:
	AddBallItem(Object* pObject) : Item(pObject){}
	void Init() { Item::Init(); }
	void Update() { Item::Update(); }
private:
	void onHit() override;
};

//ブロック消す
class ClearBlockItem : public Item
{
public:
	ClearBlockItem(Object* pObject) : Item(pObject) {}
	void Init() { Item::Init(); }
	void Update() { Item::Update(); }
private:
	void onHit() override;
};

//補助バー
class AssistBarItem : public Item
{
public:
	AssistBarItem(Object* pObject) : Item(pObject) {}
	void Init() { Item::Init(); }
	void Update() { Item::Update(); }
private:
	void onHit() override;
};

//ハート増加
class AddHeartItem : public Item
{
public:
	AddHeartItem(Object* pObject) : Item(pObject) {}
	void Init() { Item::Init(); }
	void Update() { Item::Update(); }
private:
	void onHit() override;
};

#endif // !_ITEM_H_
