//=======================================================================================================
//
//ステージの処理[stage.cpp]
// Author MurataShinnosuke
//
//=======================================================================================================
#include "stage.h"
#include "prefab.h"
#include "waypoint.h"

//=======================================================================================================
//読み込み
//=======================================================================================================
void Stage::Load(Stage::TYPE type)
{
	//種類によって処理を変える
	switch (type)
	{
	case Stage::TYPE::IS_DEMO:
		//デモステージを読み込む
		this->LoadDemo();

		break;
	}
}

//=======================================================================================================
//読み込み
//=======================================================================================================
void Stage::LoadDemo()
{
	//床を生成する
	FieldPrefab::Create();

	//ブロックを生成する
	BlockPrefab::Create({ 100.0f, 0.0f, 0.0f });
	BlockPrefab::Create({ -100.0f, 0.0f, 0.0f });

	//キノコを生成する
	MushroomPrefab::Create({ 35.0f, 0.0f, 0.0f });
	MushroomPrefab::Create({ -35.0f, 0.0f, 0.0f });

	//プレイヤーを生成する
	PlayerPrefab::Create({ 0.0f, 0.0f, -250.0f });
	CursorPrefab::Create({ 0.0f, 0.01f, -250.0f });
	NumInsectPrefab::Create();

	//虫を生成する
	InsectPrefab::Create({ -60.0f, 0.0f, -300.0f });
	InsectPrefab::Create({ -30.0f, 0.0f, -300.0f });
	InsectPrefab::Create({ 0.0f, 0.0f, -300.0f });
	InsectPrefab::Create({ 30.0f, 0.0f, -300.0f });
	InsectPrefab::Create({ 60.0f, 0.0f, -300.0f });

	//フルーツを生成する
	FruitPrefab::Create({ 100.0f, 0.0f, -200.0f }, 1);
	FruitPrefab::Create({ -100.0f, 0.0f, -200.0f }, 1);

	//ゼリー
	JellyPrefab::Create({ 100.0f, 0.0f, -100.0f }, 1);

	//かごを生成する
	BasketPrefab::Create({ -100.0f, 0.02f, 250.0f });

	//巣を生成する
	NestPrefab::Create({ 100.0f, 0.0f, 250.0f });

	//スコアを生成する
	ScorePrefab::Create();

	//ウェイポイントの設定
	{
		//ウェイポイントを生成する
		Waypoint* pWaypoint0 = new Waypoint({ { -100.0f, 0.0f, 250.0f } });
		WeypointPrefab::Create({ -100.0f, 100.0f, 250.0f }, 0, "waypoint_00.png");

		//Waypoint* pWaypoint1 = new Waypoint({ { 0.0f, 0.0f, 0.0f } });
		//WeypointPrefab::Create({ 0.0f, 100.0f, 0.0f }, 1, "waypoint_01.png");

		Waypoint* pWaypoint2 = new Waypoint({ { -200.0f, 0.0f, 0.0f } });
		WeypointPrefab::Create({ -200.0f, 100.0f, 0.0f }, 2, "waypoint_02.png");

		Waypoint* pWaypoint3 = new Waypoint({ { -200.0f, 0.0f, -250.0f } });
		WeypointPrefab::Create({ -200.0f, 100.0f, -250.0f }, 3, "waypoint_03.png");

		Waypoint* pWaypoint4 = new Waypoint({ { 0.0f, 0.0f, -250.0f } });
		WeypointPrefab::Create({ 0.0f, 100.0f, -250.0f }, 4, "waypoint_04.png");

		Waypoint* pWaypoint5 = new Waypoint({ { 200.0f, 0.0f, -250.0f } });
		WeypointPrefab::Create({ 200.0f, 100.0f, -250.0f }, 5, "waypoint_05.png");

		Waypoint* pWaypoint6 = new Waypoint({ { 200.0f, 0.0f, 0.0f } });
		WeypointPrefab::Create({ 200.0f, 100.0f, 0.0f }, 6, "waypoint_06.png");

		Waypoint* pWaypoint7 = new Waypoint({ { 100.0f, 0.0f, 250.0f } });
		WeypointPrefab::Create({ 100.0f, 100.0f, 250.0f }, 7, "waypoint_07.png");

		//ウェイポイントを繋げる
		//pWaypoint0->AddConnect(pWaypoint1);
		//pWaypoint1->AddConnect(pWaypoint0);

		pWaypoint0->AddConnect(pWaypoint2);
		pWaypoint2->AddConnect(pWaypoint0);

		pWaypoint0->AddConnect(pWaypoint7);
		pWaypoint7->AddConnect(pWaypoint0);

		pWaypoint2->AddConnect(pWaypoint3);
		pWaypoint3->AddConnect(pWaypoint2);

		pWaypoint3->AddConnect(pWaypoint4);
		pWaypoint4->AddConnect(pWaypoint3);

		pWaypoint4->AddConnect(pWaypoint5);
		pWaypoint5->AddConnect(pWaypoint4);

		pWaypoint5->AddConnect(pWaypoint6);
		pWaypoint6->AddConnect(pWaypoint5);

		pWaypoint6->AddConnect(pWaypoint7);
		pWaypoint7->AddConnect(pWaypoint6);
	}
}