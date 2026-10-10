//========================================================================================================
//
//虫の運搬状態の処理[insect_state_carry.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "insect_state.h"
#include "insect.h"
#include "cargo.h"
#include "rigidbody.h"

//========================================================================================================
//ハンドル
//========================================================================================================
void InsectCarryState::Handle()
{
	//運搬オブジェクトの回収フラグを確認する
	if (this->m_pCarry->GetComponent<Cargo>()->GetCollect())
	{//回収されている場合
		//ニュートラル状態に変更する
		this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectNeutralState(this->GetIncect()));
	}
	else
	{
		//運搬オブジェクトの移動量を自身にも加算する
		D3DXVECTOR3 moveCarry = this->m_pCarry->GetComponent<Rigidbody>()->GetMove();
		this->GetIncect()->GetComponent<Transform>()->AddPos(moveCarry);
	}
}

//========================================================================================================
//呼ぶ
//========================================================================================================
void InsectCarryState::Call(Object* pPlayer)
{
	//状態を変更する
	this->m_pCarry->GetComponent<Cargo>()->AddILiftPower(-1);   //力を減らす
	this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectFollowState(this->GetIncect(), pPlayer));
}