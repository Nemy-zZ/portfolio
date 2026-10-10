//========================================================================================================
//
//当たり判定の処理[physics.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "physics.h"
#include "collider.h"
#include "rigidbody.h"

//静的メンバ変数を初期化する
Physics* Physics::m_pInstance = nullptr;

//========================================================================================================
//更新処理
//========================================================================================================
void Physics::Calc()
{
	//コライダーを取得する
	std::vector<Collider*> vCollider = Component::Get<Collider>();

	//コライダーの数分繰り返す
	bool isCollision = false;   //衝突フラグ
	for (unsigned int i = 0; i < vCollider.size(); i++)
	{
		//トリガーか確認する
		if (vCollider[i]->GetTrigger())
		{//トリガーの場合
			//次のループに移行する
			continue;
		}

		//リジットボディが付いているか確認する
		if (vCollider[i]->GetObj()->GetComponent<Rigidbody>() == nullptr)
		{//トリガーの場合
			//次のループに移行する
			continue;
		}

		//コライダーをの数分繰り返す
		for (unsigned int j = 0; j < vCollider.size(); j++)
		{
			//コライダーが同じか確認する
			if (i == j)
			{
				//次のループに移行する
				continue;
			}

			//ボックスとボックスの衝突判定処理を行う
			if (this->BoxToBox(((BoxCollider*)vCollider[i]), (BoxCollider*)vCollider[j]))
			{
				//衝突フラグを立てる
				isCollision = true;
			}
		}
		//リジットボディを確認する
		Rigidbody* pRigidbody = vCollider[i]->GetObj()->GetComponent<Rigidbody>();
		if (pRigidbody != nullptr)
		{
			//衝突フラグを確認する
			if (isCollision == true)
			{
				//空中フラグを下ろす
				pRigidbody->SetAir(false);
			}
			else
			{
				//空中フラグを立てる
				pRigidbody->SetAir(true);
			}
		}

	}
}

//========================================================================================================
//ボックスとボックスの衝突判定処理
//========================================================================================================
bool Physics::BoxToBox(BoxCollider* pBoxCollider, BoxCollider* pBoxColliderTarget)
{
	//範囲を取得する
	BoxCollider::Range range = pBoxCollider->GetRange();
	BoxCollider::Range rangeTarget = pBoxColliderTarget->GetRange();

	bool isCollison = false;   //衝突フラグ

	//上に衝突しているか確認する
	if (rangeTarget.posMax.y > range.posMin.y &&
		rangeTarget.posMax.y <= range.posMinOld.y &&
		rangeTarget.posMax.x > range.posMinOld.x &&
		rangeTarget.posMin.x < range.posMaxOld.x &&
		rangeTarget.posMax.z > range.posMinOld.z &&
		rangeTarget.posMin.z < range.posMaxOld.z)
	{//上に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_TOP);

		//衝突フラグを立てる
		isCollison = true;
	}

	//下に衝突しているか確認する
	if (rangeTarget.posMin.y < range.posMax.y &&
		rangeTarget.posMin.y >= range.posMaxOld.y &&
		rangeTarget.posMax.x > range.posMinOld.x &&
		rangeTarget.posMin.x < range.posMaxOld.x &&
		rangeTarget.posMax.z > range.posMinOld.z &&
		rangeTarget.posMin.z < range.posMaxOld.z)
	{//下に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_BOTTOM);

		//衝突フラグを立てる
		isCollison = true;
	}

	//右に衝突しているか確認する
	if (rangeTarget.posMax.x > range.posMin.x &&
		rangeTarget.posMax.x <= range.posMinOld.x &&
		rangeTarget.posMax.y > range.posMinOld.y &&
		rangeTarget.posMin.y < range.posMaxOld.y &&
		rangeTarget.posMax.z > range.posMinOld.z &&
		rangeTarget.posMin.z < range.posMaxOld.z)
	{//右に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_RIGHT);

		//衝突フラグを立てる
		isCollison = true;
	}

	//左に衝突しているか確認する
	if (rangeTarget.posMin.x < range.posMax.x &&
		rangeTarget.posMin.x >= range.posMaxOld.x &&
		rangeTarget.posMax.y > range.posMinOld.y &&
		rangeTarget.posMin.y < range.posMaxOld.y &&
		rangeTarget.posMax.z > range.posMinOld.z &&
		rangeTarget.posMin.z < range.posMaxOld.z)
	{//左に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_LEFT);
		//衝突フラグを立てる
		isCollison = true;
	}

	//奥に衝突しているか確認する
	if (rangeTarget.posMax.z > range.posMin.z &&
		rangeTarget.posMax.z <= range.posMinOld.z &&
		rangeTarget.posMax.y > range.posMinOld.y &&
		rangeTarget.posMin.y < range.posMaxOld.y &&
		rangeTarget.posMax.x > range.posMinOld.x &&
		rangeTarget.posMin.x < range.posMaxOld.x)
	{//奥に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_BACK);

		//衝突フラグを立てる
		isCollison = true;
	}

	//前に衝突しているか確認する
	if (rangeTarget.posMin.z < range.posMax.z &&
		rangeTarget.posMin.z >= range.posMaxOld.z &&
		rangeTarget.posMax.y > range.posMinOld.y &&
		rangeTarget.posMin.y < range.posMaxOld.y &&
		rangeTarget.posMax.x > range.posMinOld.x &&
		rangeTarget.posMin.x < range.posMaxOld.x)
	{//手前に衝突した場合
		//衝突時の処理を行う
		pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_FRONT);

		//衝突フラグを立てる
		isCollison = true;
	}

	//衝突フラグを確認する
	if (isCollison == false)
	{//衝突フラグが立っていない場合
		//中に入り込んでいるか確認する
		if (rangeTarget.posMaxOld.y > range.posMinOld.y &&
			rangeTarget.posMinOld.y < range.posMaxOld.y &&
			rangeTarget.posMaxOld.x > range.posMinOld.x &&
			rangeTarget.posMinOld.x < range.posMaxOld.x &&
			rangeTarget.posMaxOld.z > range.posMinOld.z &&
			rangeTarget.posMinOld.z < range.posMaxOld.z)
		{//中に入り込んでいる場合
			//衝突時の処理を行う
			pBoxCollider->Collision(pBoxColliderTarget, Physics::CONTACT::IS_IN);

			//衝突フラグを立てる
			isCollison = true;
		}
	}

	//衝突フラグを返す
	return isCollison;
}

//========================================================================================================
//生成
//========================================================================================================
void Physics::Create()
{
	//インスタンスを確認する
	if (Physics::m_pInstance != nullptr)
	{
		//処理終了
		return;
	}

	//インスタンスを生成する
	Physics::m_pInstance = new Physics;
}