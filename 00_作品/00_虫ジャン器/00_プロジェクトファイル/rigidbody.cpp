//========================================================================================================
//
//リジットボディの処理[rigidbody.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "rigidbody.h"
#include "object.h"

//========================================================================================================
//更新
//========================================================================================================
void Rigidbody::Update()
{
	//空中フラグを確認する
	if (this->m_isAir == true)
	{//空中フラグが立っている場合
		//空中用の摩擦で慣性を計算する
		this->m_move.x += (0.0f - this->m_move.x) * this->m_fFrictionAir;
		this->m_move.y += (0.0f - this->m_move.y) * this->m_fFrictionAir;
	}
	else
	{//立っていない場合
		//慣性を計算する
		this->m_move.x += (0.0f - this->m_move.x) * this->m_fFriction;
		this->m_move.y += (0.0f - this->m_move.y) * this->m_fFriction;
	}

	//重力を加算する
	this->m_move.y += this->m_fGravity;;

	//移動量をトランスフォームの位置に加算する
	this->GetObj()->GetComponent<Transform>()->AddPos(this->m_move);
}