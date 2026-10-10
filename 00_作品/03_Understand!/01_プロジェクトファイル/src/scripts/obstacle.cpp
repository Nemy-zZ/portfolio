//------------------------------------------------------------
// @file		obstacle.cpp
// @brief	障害物
//------------------------------------------------------------
#include "obstacle.h"
#include "scenes/game.h"
#include "ball.h"
#include "components/3d/model.h"

//=============================================================
// 初期化
//=============================================================
void Obstacle::Init()
{
	m_type = OBSTACLETYPE::OBSTACLE_WHITE;
	
	// 破棄タイマー
	m_destroyTimer = Benlib::RandomFloat(3.0f, 10.0f);
}

//=============================================================
// 更新
//=============================================================
void Obstacle::Update()
{
	auto ball = AppScene->GetScene<GameScene>()->GetBall()->GetComponent<Ball>()->GetType();

	if ((m_type == OBSTACLETYPE::OBSTACLE_WHITE && ball == Ball::BALLTYPE::BALL_WHITE) ||
		(m_type == OBSTACLETYPE::OBSTACLE_BLACK && ball == Ball::BALLTYPE::BALL_BLACK))
	{ // 判定あり
		gameObject->GetComponent<Collision>()->SetTrigger(false);
	}
	else
	{
		gameObject->GetComponent<Collision>()->SetTrigger(true);
	}

	// 反射設定
	gameObject->GetComponent<Collision>()->GetCollision()->setRestitution(1.0f);

	// 色変更
	auto model = gameObject->FindChildrenByName("model");
	model->GetComponent<Model>()->SetColorEnabled(true);
	model->GetComponent<Model>()->SetColorBlack(m_type == OBSTACLETYPE::OBSTACLE_BLACK);

	// 削除タイマー
	if (m_destroyTimer <= 0.0f)
	{
		if (gameObject->name != "Frame")
		{
			Destroy(gameObject);
		}
	}
	else
	{
		m_destroyTimer -= Main::GetInstance().GetDeltaTime();
	}
}


