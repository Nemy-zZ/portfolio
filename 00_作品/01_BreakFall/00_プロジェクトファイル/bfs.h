//========================================================================================================
//
//幅優先探索[bfs.h]
// Author MurataShinnosuke
//
//========================================================================================================
#ifndef _BFS_H_
#define _BFS_H_

#include "main.h"
#include "waypoint.h"

//幅優先探索
class Bfs
{
public:
	//ノードの構造体
	struct Node
	{
		Waypoint* pWaypoint;   //ウェイポイント
		int nStep;   //ステップ
		int nFrom;   //伝播
	};

	//コンストラクタ
	Bfs()
	{
	}

	//デストラクタ
	~Bfs()
	{
	}

	static std::vector<D3DXVECTOR3> CalcRoute(Waypoint* pStart, Waypoint* pGoal);   //ルートの演算

private:
};

#endif // !_BFS_H_
