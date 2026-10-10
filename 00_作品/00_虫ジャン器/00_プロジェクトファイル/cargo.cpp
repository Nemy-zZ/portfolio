//========================================================================================================
//
//貨物の処理[cargo.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "cargo.h"
#include "benlib.h"
#include "insect.h"
#include "insect_state.h"
#include "transform.h"
#include "rigidbody.h"
#include "bfs.h"

//静的メンバ変数を初期化する
const int Cargo::FRAME_COLLECT = 60;   //回収までのフレーム
const float Cargo::DIRE_WAYPOINT = 10.0f;   //ウェイポイントに到達したと判断する距離
const float Cargo::MOVE = 0.5f;   //移動量
const float Cargo::SUB_SCALE = 0.05f;   //拡大率の減少量

//========================================================================================================
//初期化処理
//========================================================================================================
void Cargo::Init()
{

}

//========================================================================================================
//更新処理
//========================================================================================================
void Cargo::Update()
{
	//回収フラグを確認する
	if (this->m_isCollect)
	{
		//拡大率を小さくする
		Transform* pTransform = this->GetObj()->GetComponent<Transform>();
		D3DXVECTOR3 scale = pTransform->GetScale();
		scale -= {Cargo::SUB_SCALE, Cargo::SUB_SCALE, Cargo::SUB_SCALE};
		if (scale.x <= 0.0f)
		{//0になった場合
			//死亡フラグを立てる
			this->GetObj()->Death();
			return;
		}

		//拡大率を設定する
		pTransform->SetScale(scale);
		return;
	}

	//持ち上げる力と重さを確認する
	if (this->m_nLiftPower >= this->m_nWeight)
	{
		//持ち上げられてるフラグを立てる
		this->m_isLift = true;

		//ルートを確認する
		if (this->m_vRoute.empty())
		{//ルートが設定されていない場合
			//1番近いウェイポイントを算出する
			std::vector<Waypoint*> vWaypoint = Waypoint::Get();
			float fDistMin = -1.0f;   //最短距離
			Waypoint* pWaypointMin = nullptr;   //最短ウェイポイント
			for (unsigned int i = 0; i < vWaypoint.size(); i++)
			{
				//距離を確認する
				float fDist = Benlib::Distance(this->GetComponent<Transform>()->GetPos(), vWaypoint[i]->GetPos()).y;
				if (fDistMin > fDist || fDistMin == -1.0f)
				{//保存されてた最短距離より短い場合
					//最短距離を上書きする
					fDistMin = fDist;
					pWaypointMin = vWaypoint[i];
				}
			}

			//かごまでのルートを計算する
			this->m_vRoute = Bfs::CalcRoute(pWaypointMin, this->m_pWaypointGoal);
			this->m_nStep = 0;
		}

		//ステップを確認する
		if ((unsigned int)this->m_nStep < this->m_vRoute.size())
		{//ルートのコンテナより小さい場合
			//持ち上げられてる力を確認する
			float fMove = Cargo::MOVE;
			if (this->m_nLiftPower >= (this->m_nWeight * 2))
			{//持ち上げられてる力が質量の2倍の場合
				//移動量を倍率をかける
				fMove *= 1.5;
			}

			//ウェイポイントに向かって移動する
			float fDire = Benlib::Direction(this->GetComponent<Transform>()->GetPos(), this->m_vRoute[this->m_nStep]).y;
			this->GetComponent<Rigidbody>()->AddMove({ sinf(fDire) * fMove, 0.0f, cosf(fDire) * fMove });
			if (Benlib::Distance(this->GetComponent<Transform>()->GetPos(), this->m_vRoute[this->m_nStep]).y <= Cargo::DIRE_WAYPOINT)
			{//ウェイポイントに到達した場合
				//次のステップする
				this->m_nStep++;
			}
		}
	}
	else
	{
		//持ち上げられてるフラグを下ろす
		this->m_isLift = false;
	}
}

//========================================================================================================
//回収
//========================================================================================================
void Cargo::Collect()
{
	//回収フラグを立てる
	this->m_isCollect = true;
}