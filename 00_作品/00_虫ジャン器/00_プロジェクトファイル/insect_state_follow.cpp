//========================================================================================================
//
//虫の追従状態の処理[insect_state_follow.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "insect_state.h"
#include "insect.h"
#include "benlib.h"
#include "transform.h"
#include "rigidbody.h"
#include "cursor.h"

//静的メンバ変数を初期化する
const float InsectFollowState::DIS_PLAYER = 75.0f;   //プレイヤーとの距離

//========================================================================================================
//ハンドル
//========================================================================================================
void InsectFollowState::Handle()
{
	//プレイヤーとの距離を確認する
	D3DXVECTOR3 pos = this->GetIncect()->GetComponent<Transform>()->GetPos();   //自身の位置
	D3DXVECTOR3 posPlayer = this->pPlayer->GetComponent<Transform>()->GetPos();   //プレイヤーの位置
	float fDisPlayer = Benlib::Distance(pos, posPlayer).y;   //距離
	if (fDisPlayer >= InsectFollowState::DIS_PLAYER)
	{
		//プレイヤーに追従する
		float fDirePlayer = Benlib::Direction(pos, posPlayer).y;   //方向
		this->GetIncect()->GetComponent<Transform>()->AddPos({ sinf(fDirePlayer) * Insect::MOVE_SPEED, 0.0f, cosf(fDirePlayer) * Insect::MOVE_SPEED });
	}
}

//========================================================================================================
//投げられる
//========================================================================================================
void InsectFollowState::Throw(D3DXVECTOR3 posThrow, float fDireThrow, float fDistCursor)
{
	//方向に移動量を加算する
	float fRateDist = fDistCursor / Cursor::MAX_DISTANCE;
	D3DXVECTOR3 move = { sinf(fDireThrow) * (Insect::MOVE_THROW * fRateDist), (Insect::MOVE_THROW * fRateDist) * 0.5f, cosf(fDireThrow) * (Insect::MOVE_THROW * fRateDist) };
	this->GetIncect()->GetComponent<Rigidbody>()->SetMove(move);
	this->GetIncect()->GetComponent<Rigidbody>()->SetAir(true);
	this->GetIncect()->GetComponent<Transform>()->SetPos({ posThrow.x, posThrow.y, posThrow.z });   //位置を設定
	this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectThrowState(this->GetIncect()));   //状態を投げられてるに変更
}