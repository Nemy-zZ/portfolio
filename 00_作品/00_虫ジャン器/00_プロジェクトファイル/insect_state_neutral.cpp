//========================================================================================================
//
//虫のニュートラル状態の処理[insect_state_neutral.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "insect_state.h"
#include "insect.h"
#include "cargo.h"
#include "obstacle.h"
#include "transform.h"
#include "benlib.h"

//静的メンバ変数を初期化する
const float	InsectNeutralState::DIS_SEARCH = 100.0f;

//========================================================================================================
//ハンドル
//========================================================================================================
void InsectNeutralState::Handle()
{
	//オブジェクトを探す
	this->SeatchObject();
}

//========================================================================================================
//呼ぶ
//========================================================================================================
void InsectNeutralState::Call(Object* pPlayer)
{
	//状態を変更する
	this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectFollowState(this->GetIncect(), pPlayer));
}

//========================================================================================================
//フルーツを探す
//========================================================================================================
void InsectNeutralState::SeatchObject()
{
	//フルーツを取得する
	std::vector<Cargo*> vCargo = Component::Get<Cargo>();

	//フルーツの数分繰り返す
	for (unsigned int i = 0; i < vCargo.size(); i++)
	{
		//フルーツとの距離を確認する
		D3DXVECTOR3 posCargo = vCargo[i]->GetObj()->GetComponent<Transform>()->GetPos();
		float fDisCargo = Benlib::Distance(this->GetIncect()->GetComponent<Transform>()->GetPos(), posCargo).y;
		if (fDisCargo <= InsectNeutralState::DIS_SEARCH)
		{//サーチ範囲内の場合
			//フルーツの回収フラグを確認する
			if (vCargo[i]->GetCollect() == false)
			{
				//移動状態に変更する
				this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectMoveState(this->GetIncect(), posCargo));
				return;   //処理終了
			}
		}
	}

	//障害物を取得する
	std::vector<Obstacle*> vObstacle = Component::Get<Obstacle>();

	//障害物の数分繰り返す
	for (unsigned int i = 0; i < vObstacle.size(); i++)
	{
		//障害物との距離を確認する
		D3DXVECTOR3 posObstacle = vObstacle[i]->GetObj()->GetComponent<Transform>()->GetPos();
		float fDisObstacle = Benlib::Distance(this->GetIncect()->GetComponent<Transform>()->GetPos(), posObstacle).y;
		if (fDisObstacle <= InsectNeutralState::DIS_SEARCH)
		{//サーチ範囲内の場合
			//障害物の破壊フラグを確認する
			if (vObstacle[i]->GetBreak() == false)
			{//破壊されていない場合
				//移動状態に変更する
				this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectMoveState(this->GetIncect(), posObstacle));
				return;   //処理終了
			}
		}
	}
}

//========================================================================================================
//フルーツを探す
//========================================================================================================
void InsectNeutralState::SearchFruit()
{
	//フルーツを取得する
	std::vector<Cargo*> vCargo = Component::Get<Cargo>();

	//フルーツの数分繰り返す
	for (unsigned int i = 0; i < vCargo.size(); i++)
	{
		//フルーツとの距離を確認する
		D3DXVECTOR3 posCargo = vCargo[i]->GetObj()->GetComponent<Transform>()->GetPos();
		float fDisCargo = Benlib::Distance(this->GetIncect()->GetComponent<Transform>()->GetPos(), posCargo).y;
		if (fDisCargo <= InsectNeutralState::DIS_SEARCH)
		{//サーチ範囲内の場合
			//フルーツの回収フラグを確認する
			if (vCargo[i]->GetCollect() == false)
			{
				//移動状態に変更する
				this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectMoveState(this->GetIncect(), posCargo));
				break;
			}
		}
	}
}

//========================================================================================================
//障害物を探す
//========================================================================================================
void InsectNeutralState::SearchObstacle()
{
	//障害物を取得する
	std::vector<Obstacle*> vObstacle = Component::Get<Obstacle>();

	//障害物の数分繰り返す
	for (unsigned int i = 0; i < vObstacle.size(); i++)
	{
		//障害物との距離を確認する
		D3DXVECTOR3 posObstacle = vObstacle[i]->GetObj()->GetComponent<Transform>()->GetPos();
		float fDisObstacle = Benlib::Distance(this->GetIncect()->GetComponent<Transform>()->GetPos(), posObstacle).y;
		if (fDisObstacle <= InsectNeutralState::DIS_SEARCH)
		{//サーチ範囲内の場合
			//障害物の破壊フラグを確認する
			if (vObstacle[i]->GetBreak() == false)
			{//破壊されていない場合
				//移動状態に変更する
				this->GetIncect()->GetComponent<Insect>()->ChangeState(new InsectMoveState(this->GetIncect(), posObstacle));
				break;   //処理を抜け出す
			}
		}
	}
}