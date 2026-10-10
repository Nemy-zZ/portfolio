//========================================================================================================
//
//かごの処理[basket.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _BASKET_H_
#define _BASKET_H_

#include "behavior.h"

//かごクラス
class Basket : public Behavior   //(親)挙動
{
public:
	//コンストラクタ
	Basket(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_pScore = nullptr;
	}

	//デストラクタ
	~Basket() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Collision(Object* pObject, Physics::CONTACT contact) override;   //衝突時

private:
	Object* m_pScore;
};

#endif // !_BAKET_H_
