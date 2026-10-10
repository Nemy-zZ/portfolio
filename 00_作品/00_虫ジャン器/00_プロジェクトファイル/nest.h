//========================================================================================================
//
//巣の処理[nest.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _NEST_H_
#define _NEST_H_

#include "behavior.h"

//巣クラス
class Nest : public Behavior   //(親)挙動
{
public:
	static const int NUM_SPAWN;   //スポーン数
	static const float MOVE_SPAWN;   //スポーンした時の移動量

	//コンストラクタ
	Nest(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Nest() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void SpawnInsect();   //虫のスポーン
	void Collision(Object* pObject, Physics::CONTACT contact) override;   //衝突時

private:
};

#endif // !_BAKET_H_
