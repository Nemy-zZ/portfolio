//==========================================================================================================
//
//挙動の処理[behavior.h]
// Author MurataShinnosuke
//
//==========================================================================================================
#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_

#include "component.h"
#include "object.h"
#include "physics.h"

//挙動
class Behavior : public Component   //(親)コンポーネント
{
public:
	//コンストラクタ
	Behavior(Object* pObject) : Component(pObject)
	{
	}

	//デストラクタ
	virtual ~Behavior() override
	{
	}

	virtual void Init() override = 0;
	virtual void Update() override = 0;
	virtual void Collision(Object* pObject, Physics::CONTACT contact) {};   //衝突処理
	virtual void EndMotion() {};   //モーションの終了処理
};

#endif // !_BEHAVIOR_H_
