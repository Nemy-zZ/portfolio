//=======================================================================================================
//
//ボールの処理[ball.h]
// Author MurataShinnosuke
//
//=======================================================================================================
#ifndef _BALL_H_
#define _BALL_H_

#include "behavior.h"

//ボールの挙動
class Ball : public Behavior   //（親）挙動
{
public:
	static const float MOVE;   //移動量

	//コンストラクタ
	Ball(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_bReflection = false;
		this->m_pHeart = nullptr;
	}

	//デストラクタ
	~Ball()
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Collision(Object* pObject, Physics::CONTACT contact) override;

private:
	void Damage();

	bool m_bReflection;   //反射フラグ
	Object* m_pHeart;   //ハート
};

#endif // !_BALL_H_
