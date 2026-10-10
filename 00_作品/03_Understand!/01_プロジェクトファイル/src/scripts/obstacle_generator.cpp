//------------------------------------------------------------
// @file		obstacle_generator.cpp
// @brief	障害物ジェネレーター
//------------------------------------------------------------
#include "obstacle_generator.h"

#include "obstacle.h"

//=============================================================
// 初期化
//=============================================================
void ObstacleGenerator::Init()
{
	m_timer = 0.0f;
}

//=============================================================
// 更新
//=============================================================
void ObstacleGenerator::Update()
{
	if (m_timer <= 0.0f)
	{
		// オブジェクト生成
		GenerateObject();

		m_timer = Benlib::RandomFloat(1.0f, 2.0f);
	}
	else
	{
		m_timer -= Main::GetInstance().GetDeltaTime();
	}
}

//=============================================================
// オブジェクト生成
//=============================================================
void ObstacleGenerator::GenerateObject()
{
	// 形を決める
	int shape = Benlib::RandomInt(0, 1);

	GameObject* obj = nullptr;

	switch (shape)
	{
	case 0:
		obj = GameObject::CreateObuseObject("data\\OBUS\\Box.obus");
		break;

	case 1:
		obj = GameObject::CreateObuseObject("data\\OBUS\\Cylinder.obus");
		break;
	}

	// 障害物
	auto obstacle = obj->AddComponent<Obstacle>();

	// 色をランダムで決める
	obstacle->SetType(Benlib::RandomInt(0, 1) == 0 ? Obstacle::OBSTACLETYPE::OBSTACLE_WHITE : Obstacle::OBSTACLETYPE::OBSTACLE_BLACK);

	// 位置を決める
	obstacle->transform->position.x = Benlib::RandomFloat(-140.0f, 140.0f);
	obstacle->transform->position.y = Benlib::RandomFloat(-25.0f, 125.0f);

	// 回転を決める
	obstacle->transform->SetEulerAngle(0.0f, 0.0f, Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f));

	// スケールを決める
	float scale = Benlib::RandomFloat(0.5f, 1.5f);
	obstacle->transform->scale = { scale, scale, scale };
}
