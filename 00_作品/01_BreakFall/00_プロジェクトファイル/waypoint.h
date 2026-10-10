//========================================================================================================
//
//ウェイポイントの処理[waypoint.h]
// Author MurataShinnosuke
//
//========================================================================================================
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include "main.h"

//ウェイポイント
class Waypoint
{
public:
	//コンストラクタ
	Waypoint(D3DXVECTOR3 pos)
	{
		//メンバ変数を初期化する
		this->m_pos = pos;   //位置
		this->m_vConnect.clear();   //繋がってるポイント

		//コンテナに保存する
		Waypoint::m_vWaypoint.push_back(this);
	}

	//デストラクタ
	~Waypoint()
	{
	}

	void AddConnect(Waypoint* pWaypoint) { this->m_vConnect.push_back(pWaypoint); };   //繋がってるポイントの追加

	D3DXVECTOR3 GetPos() { return this->m_pos; };   //位置の取得
	std::vector<Waypoint*> GetConnect() { return this->m_vConnect; };   //繋がってるポイントの取得

	//順番の取得
	int GetOder() 
	{
		//コンテナの数分繰り返す
		for (unsigned int i = 0; i < Waypoint::m_vWaypoint.size(); i++)
		{
			//ウェイポイントを確認する
			if (Waypoint::m_vWaypoint[i] == this)
			{
				//順番を返す
				return i;
			}
		}

		//-1を返す
		return -1;
	}

	//一括解放
	static void ReleaseAll() 
	{
		//ウェイポイントの数分繰り返す
		for (unsigned int i = 0; i < Waypoint::m_vWaypoint.size(); i++)
		{
			//ウェイポイントを破棄する
			delete Waypoint::m_vWaypoint[i];
		}

		//コンテナのクリアする
		Waypoint::m_vWaypoint.clear();
	}

	static std::vector<Waypoint*> Get() { return Waypoint::m_vWaypoint; };   //コンテナの取得

private:
	D3DXVECTOR3 m_pos;   //位置
	std::vector<Waypoint*> m_vConnect;   //繋がってるポイント

	static std::vector<Waypoint*> m_vWaypoint;   //コンテナ
};

#endif // !_WAYPOINT_H_
