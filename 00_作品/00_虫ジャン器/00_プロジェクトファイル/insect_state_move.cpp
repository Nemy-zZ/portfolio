//========================================================================================================
//
//虫のニュートラル状態の処理[insect_state_move.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "insect_state.h"
#include "insect.h"
#include "transform.h"
#include "cargo.h"
#include "obstacle.h"
#include "benlib.h"

//========================================================================================================
//ハンドル
//========================================================================================================
void InsectMoveState::Handle()
{
	//移動先に向かって移動する
	Transform* pTransfrom = this->GetIncect()->GetComponent<Transform>();
	float fDire = Benlib::Direction(pTransfrom->GetPos(), this->m_posTarget).y;
	pTransfrom->AddPos({ sinf(fDire) * Insect::MOVE_SPEED, 0.0f, cosf(fDire) * Insect::MOVE_SPEED });
}

//========================================================================================================
//呼ぶ
//========================================================================================================
void InsectMoveState::Call(Object* pPlayer)
{
	//状態を変更する
	this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectFollowState(this->GetIncect(), pPlayer));
}

//========================================================================================================
//衝突時
//========================================================================================================
void InsectMoveState::Collision(Object* pObject)
{
	//衝突したオブジェクトのタグを確認する
	Object::TAG tagObject = pObject->GetTag();
	if (tagObject == Object::TAG::IS_FRUIT || tagObject == Object::TAG::IS_JELLY)
	{//フルーツの場合
		//回収フラグを確認する
		if (!pObject->GetComponent<Cargo>()->GetCollect())
		{
			//状態を運搬状態に変更する
			this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectCarryState(this->GetIncect(), pObject));

			//フルーツの持ち上げられる力を加算する
			pObject->GetComponent<Cargo>()->AddILiftPower(1);
		}
	}
	else if (tagObject == Object::TAG::IS_OBSTACLE)
	{//障害物の場合
		//オブジェクトを破壊フラグを確認する
		if (!pObject->GetComponent<Obstacle>()->GetBreak())
		{//破壊されて居なかったら
			//攻撃状態に変更する
			this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectAttackState(this->GetIncect(), pObject));
		}
	}
}