//========================================================================================================
//
//幅優先探索[bfs.cpp]
// Author MurataShinnosuke
//
//========================================================================================================
#include "bfs.h"

//========================================================================================================
//ルートの演算
//========================================================================================================
std::vector<D3DXVECTOR3> Bfs::CalcRoute(Waypoint* pStart, Waypoint* pGoal)
{
    //ウェイポイントを取得する
    std::vector<Waypoint*> vWaypoint = Waypoint::Get();
    std::vector<Bfs::Node> vNode(vWaypoint.size(), { nullptr, -1, -1 });   //ノード
    for (unsigned int i = 0; i < vWaypoint.size(); i++)
    {
        //ノードにウェイポイントを保存する
        vNode[i].pWaypoint = vWaypoint[i];
    }

    //スタートウェイポイントを発見したノードのコンテナに保存する
    std::vector<Bfs::Node> vNodeFind = { {pStart, 0, -1} };

    //発見したノードが空になるまで繰り返す
    while (!vNodeFind.empty())
    {
        //ゴールウェイポイントか確認する
        if (vNodeFind.front().pWaypoint == pGoal)
        {//ゴールウェイポイントの場合
            //ループを抜け出す
            break;
        }

        //発見したノードのコンテナの先頭を取り除く
        Bfs::Node nodeFront = vNodeFind.front();
        vNodeFind.erase(vNodeFind.begin());

        //繋がってるウェイポイントを取得する
        std::vector<Waypoint*> vWaypointConnect = nodeFront.pWaypoint->GetConnect();
        for (unsigned int i = 0; i < vWaypointConnect.size(); i++)
        {
            //既に発見しているか確認する
            int nOder = vWaypointConnect[i]->GetOder();
            if (vNode[nOder].nStep != -1)
            {//発見している場合
                //次のループに移行する
                continue;
            }

            //ノードにを発見した状態にする
            vNode[nOder].nStep = nodeFront.nStep + 1;
            vNode[nOder].nFrom = nodeFront.pWaypoint->GetOder();

            //繋がってるノードを発見したノードのコンテナに保存する
            vNodeFind.push_back({ vWaypointConnect[i], nodeFront.nStep + 1, nodeFront.pWaypoint->GetOder() });
        }
    }

    //返す用の位置のコンテナにゴールの位置を保存する
    std::vector<D3DXVECTOR3> vPosRoute;
    vPosRoute.push_back(vNodeFind.front().pWaypoint->GetPos());
    int nId = vNodeFind.front().nFrom;   //次の番号を保存する

    //繰り返す
    while (1)
    {
        //返す用のコンテナに経由ウェイポイントの位置を保存する
        vPosRoute.insert(vPosRoute.begin(), vNode[nId].pWaypoint->GetPos());
        if (vNode[nId].pWaypoint == pStart)
        {//スタート地点の場合
            //ループを抜け出す
            break;
        }

        //次の番号を保存する
        nId = vNode[nId].nFrom;
    }

	//ルートを返す
	return vPosRoute;
}