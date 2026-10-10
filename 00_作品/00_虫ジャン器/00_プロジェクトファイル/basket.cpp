//========================================================================================================
//
//かごの処理[basket.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "basket.h"
#include "cargo.h"
#include "transform.h"
#include "benlib.h"
#include "rigidbody.h"
#include "waypoint.h"
#include "score.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Basket::Init()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void Basket::Update()
{
	//スコアを確認する
	if (this->m_pScore == nullptr)
	{//スコアがない場合
		//スコアを検索する
		this->m_pScore = Object::Find("Score");
	}

	//貨物を取得する
	std::vector<Cargo*> vCargo = Component::Get<Cargo>();

	//貨物の数分繰り返す
	for (unsigned int i = 0; i < vCargo.size(); i++)
	{
		//タグを確認する
		if (vCargo[i]->GetObj()->GetTag() == Object::TAG::IS_FRUIT)
		{//フルーツの場合
			//貨物に自身の位置を設定する
			vCargo[i]->SetWaypointGoal(Waypoint::Get()[0]);
		}
	}
}

//========================================================================================================
//衝突時
//========================================================================================================
void Basket::Collision(Object* pObject, Physics::CONTACT contact)
{
	//衝突したオブジェクトのタグを確認する
	if (pObject->GetTag() == Object::TAG::IS_FRUIT)
	{//貨物の場合
		//回収フラグを確認する
		if (!pObject->GetComponent<Cargo>()->GetCollect())
		{//回収されていない場合
			//貨物を回収する
			pObject->GetComponent<Cargo>()->Collect();
			pObject->GetComponent<Rigidbody>()->AddMove(-pObject->GetComponent<Rigidbody>()->GetMove());   //移動量を打ち消す
			this->m_pScore->GetComponent<Score>()->Add(100);
		}
	}
}