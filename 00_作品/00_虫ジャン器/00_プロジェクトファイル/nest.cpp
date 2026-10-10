//========================================================================================================
//
//巣の処理[nest.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "nest.h"
#include "cargo.h"
#include "prefab.h"
#include "transform.h"
#include "benlib.h"
#include "rigidbody.h"

//静的メンバ変数を初期化する
const int Nest::NUM_SPAWN = 5;
const float Nest::MOVE_SPAWN = 1.0f;

//========================================================================================================
//初期化処理
//========================================================================================================
void Nest::Init()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void Nest::Update()
{
	//貨物を取得する
	std::vector<Cargo*> vCargo = Component::Get<Cargo>();

	//貨物の数分繰り返す
	for (unsigned int i = 0; i < vCargo.size(); i++)
	{
		//タグを確認する
		if (vCargo[i]->GetObj()->GetTag() == Object::TAG::IS_JELLY)
		{//フルーツの場合
			//貨物に自身の位置を設定する
			vCargo[i]->SetWaypointGoal(Waypoint::Get()[6]);
		}
	}
}

//========================================================================================================
//衝突時
//========================================================================================================
void Nest::Collision(Object* pObject, Physics::CONTACT contact)
{
	//衝突したオブジェクトのタグを確認する
	if (pObject->GetTag() == Object::TAG::IS_JELLY)
	{//貨物の場合
		if (!pObject->GetComponent<Cargo>()->GetCollect())
		{//回収されていない場合
			//貨物を回収する
			pObject->GetComponent<Cargo>()->Collect();
			pObject->GetComponent<Rigidbody>()->AddMove(-pObject->GetComponent<Rigidbody>()->GetMove());   //移動量を打ち消す

			//虫をスポーンさせる
			this->SpawnInsect();
		}
	}
}

//========================================================================================================
//虫のスポーン
//========================================================================================================
void Nest::SpawnInsect()
{
	//スポーン数分繰り返す
	for (int i = 0; i < Nest::NUM_SPAWN; i++)
	{
		//虫を生成する
		D3DXVECTOR3 pos = this->GetComponent<Transform>()->GetPos();
		Object* pInsect = InsectPrefab::Create({ pos.x, pos.y + 100.0f, pos.z });
		float fDireSpawn = Benlib::CreateRand((int)D3DX_PI, 0);
		pInsect->GetComponent<Rigidbody>()->AddMove({ sinf(fDireSpawn) * Nest::MOVE_SPAWN, 100.0f, cosf(fDireSpawn) * Nest::MOVE_SPAWN });
	}
}