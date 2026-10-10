//========================================================================================================
//
//貨物の処理[cargo.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _CARGO_H_
#define _CARGO_H_

#include "behavior.h"
#include "waypoint.h"

//フルーツクラス
class Cargo : public Behavior   //(親)挙動
{
public:
	static const int FRAME_COLLECT;   //回収までのフレーム
	static const float DIRE_WAYPOINT;   //ウェイポイントまでの距離
	static const float MOVE;   //移動量
	static const float SUB_SCALE;   //拡大率の減少量

	//コンストラクタ
	Cargo(Object* pObject, int nWeight) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nWeight = nWeight;   //重さ
		this->m_nLiftPower = 0;   //持ち上げる力
		this->m_isLift = false;   //持ち上げられてるフラグ
		this->m_pWaypointGoal = nullptr;
		this->m_isCollect = false;   //回収フラグ
		this->m_nFrameCollect = 0;   //回収フレーム
		this->m_vRoute.clear();   //ルート
		this->m_nStep = 0;   //ステップ
	}

	//デストラクタ
	~Cargo() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Collect();   //回収
	void AddILiftPower(int nAddPower) { this->m_nLiftPower += nAddPower; };   //持ち上げる力の加算

	void SetWaypointGoal(Waypoint* Waypoint) { this->m_pWaypointGoal = Waypoint; };   //かごの位置の設定

	int GetWeight() { return this->m_nWeight; };   //重さの取得
	bool GetLift() { return this->m_isLift; };   //持ち上げられてるフラグの取得
	bool GetCollect() { return this->m_isCollect; };   //回収フラグ

private:
	int m_nWeight;   //重さ
	int m_nLiftPower;   //持ち上げる力
	bool m_isLift;   //持ち上げられてるフラグ
	Waypoint* m_pWaypointGoal;   //ウェイポイント
	bool m_isCollect;   //回収フラグ
	int m_nFrameCollect;   //回収フレーム
	std::vector<D3DXVECTOR3> m_vRoute;   //ルート
	int m_nStep;   //ステップ
};

#endif // !_CARGO_H_
