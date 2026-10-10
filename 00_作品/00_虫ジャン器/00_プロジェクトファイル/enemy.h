//=====================================================================================================================
//
//敵の処理[enemy.h]
// Author MurataShinosuke
//
//=====================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "behavior.h"

//敵
class Enemy : public Behavior   //(親)挙動
{
public:
	//コンストラクタ
	Enemy(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Enemy()
	{
	}

	void Init();   //初期化
	void Update();   //更新

private:

};

#endif // !_ENEMY_H_
