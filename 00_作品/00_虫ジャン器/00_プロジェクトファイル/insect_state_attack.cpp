//========================================================================================================
//
//虫の攻撃状態の処理[insect_state_attack.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "insect_state.h"
#include "insect.h"
#include "rigidbody.h"
#include "obstacle.h"

//静的メンバ変数を初期化する
const int InsectAttackState::CNT_ATTACK = 30;   //攻撃カウンター

//========================================================================================================
//ハンドル
//========================================================================================================
void InsectAttackState::Handle()
{
	//攻撃中のオブジェクトを確認する
	if (this->m_pObjAttack->GetComponent<Obstacle>()->GetBreak())
	{//アクティブじゃない場合
		//ニュートラル状態に変更する
		this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectNeutralState(this->GetIncect()));
		return;   //処理終了
	}

	//攻撃カウンターを進める
	this->m_nCntAttack++;
	if (this->m_nCntAttack >= InsectAttackState::CNT_ATTACK)
	{//攻撃カウンターが到達したら
		//攻撃する
		this->m_pObjAttack->GetComponent<Obstacle>()->Hit(1);
		this->m_nCntAttack = 0;   //攻撃カウンターをリセット
	}
}

//========================================================================================================
//呼ぶ
//========================================================================================================
void InsectAttackState::Call(Object* pPlayer)
{
	//状態を変更する
	this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectFollowState(this->GetIncect(), pPlayer));
}