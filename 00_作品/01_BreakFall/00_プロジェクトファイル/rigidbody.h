//========================================================================================================
//
//リジットボディの処理[rigidbody.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "component.h"
#include "object.h"
#include "transform.h"

//リジットボディ
class Rigidbody : public Component
{
public:
	//コンストラクタ
	Rigidbody(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_move = { 0.0f, 0.0f, 0.0f };   //移動量
		this->m_isAir = false;   //空中フラグ
		this->m_fGravity = 1.0f;   //重力
		this->m_fFriction = 0.5f;   //摩擦
		this->m_fFrictionAir = this->m_fFriction;
	}

	//デストラクタ
	~Rigidbody() override
	{
	}

	//更新処理
	void Update() override
	{
		//空中フラグを確認する
		if (this->m_isAir == true)
		{//空中フラグが立っている場合
			//空中用の摩擦で慣性を計算する
			this->m_move.x += (0.0f - this->m_move.x) * this->m_fFrictionAir;
			this->m_move.z += (0.0f - this->m_move.z) * this->m_fFrictionAir;
		}
		else
		{//立っていない場合
			//慣性を計算する
			this->m_move.x += (0.0f - this->m_move.x) * this->m_fFriction;
			this->m_move.z += (0.0f - this->m_move.z) * this->m_fFriction;
		}

		//重力を加算する
		this->m_move.y -= this->m_fGravity;

		//移動量をトランスフォームの位置に加算する
		this->GetObj()->GetComponent<Transform>()->AddPos(this->m_move);
	}

	void SetMove(D3DXVECTOR3 move) { this->m_move = move; };   //移動量の設定
	void AddMove(D3DXVECTOR3 move) { this->m_move += move; };   //移動量の加算処理

	void SetAir(bool isAir) { this->m_isAir = isAir; };
	void SetGravity(float fGravity) { this->m_fGravity = fGravity; };   //重力
	void SetFriction(float fFriction) { this->m_fFriction = fFriction; };   //摩擦
	void SetFrictionAir(float fFriction) { this->m_fFrictionAir = fFriction; };   //空中用の摩擦

	D3DXVECTOR3 GetMove() { return this->m_move; };   //移動量

private:
	D3DXVECTOR3 m_move;   //移動量
	bool m_isAir;   //空中フラグ
	float m_fGravity;   //重力
	float m_fFriction;   //摩擦
	float m_fFrictionAir;   //空中での摩擦
};

#endif // !_RIGIDBODY_H_