//========================================================================================================
//
//当たり判定の処理[physics.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "main.h"

//前方宣言
class Collider;
class BoxCollider;
class SphereCollider;
//当たり判定クラス
class Physics
{
public:
	typedef enum
	{
		IS_TOP = 0,
		IS_BOTTOM,
		IS_RIGHT,
		IS_LEFT,
		IS_FRONT,
		IS_BACK,
		IS_IN,
	}CONTACT;   //衝突位置

	void Calc();   //演算処理

	static void Create();   //生成処理
	static void Destroy() { delete Physics::m_pInstance; Physics::m_pInstance = nullptr; };   //破棄
	static Physics* Get() { return Physics::m_pInstance; };   //取得

private:
	//コンストラクタ
	Physics()
	{
	}

	//デストラクタ
	~Physics()
	{
	}

	bool BoxToBox(BoxCollider* pBoxCollider, BoxCollider* pBoxColliderTarget);
	//void SphereToSphere(SphereCollider* pSphereCollider, SphereCollider* pSphereColliderTarget);

	static Physics* m_pInstance;   //インスタンス
};

#endif // !_PHYSICS_H_
