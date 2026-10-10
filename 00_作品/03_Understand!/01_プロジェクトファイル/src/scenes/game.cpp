//------------------------------------------------------------
// @file		game.cpp
// @brief	ゲームシーン
//------------------------------------------------------------
#include "game.h"

#include "components/3d/camera.h"
#include "components/3d/light.h"
#include "components/3d/model.h"
#include "scripts/player.h"
#include "scenes/result.h"
#include "scripts/ball.h"
#include "scripts/obstacle.h"
#include "scripts/obstacle_generator.h"
#include "components/3d/billboard.h"
#include "components/2d/text.h"

//=============================================================
// 初期化
//=============================================================
void GameScene::Init()
{
	m_isStarted = false;
	m_recordTime = 0.0f;
	m_stopCount = 0;

	// ライトを作成する
	GameObject* light = new GameObject("Light", TAG_NODESTROY);
	D3DLight::SetDefaultLight(light);

	// 背景
	GameObject* bg = new GameObject();
	bg->AddComponent<Billboard>()->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	bg->transform->size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	bg->transform->position.z = 300.0f;

	// 障害物ジェネレーターの初期化
	ObstacleGenerator::GetInstance().Init();
	
	// ボール生成
	m_ball = GameObject::CreateObuseObject("data\\OBUS\\Ball.obus");
	m_ball->GetComponent<RigidBody>()->GetRigidBody()->setRestitution(1.0f);
	m_ball->GetComponent<RigidBody>()->SetAlwayActive(true);
	m_ball->AddComponent<Ball>();
	m_ball->transform->position.y = 48.0f;

	// フレーム
	auto wall = GameObject::CreateObuseObject("data\\OBUS\\Frame.obus");
	wall->AddComponent<Obstacle>();
	wall->transform->scale = { 0.52f, 0.52f, 0.05f };
	wall->transform->position.y = 49.0f;

	wall = GameObject::CreateObuseObject("data\\OBUS\\Frame.obus");
	wall->AddComponent<Obstacle>();
	wall->transform->SetEulerAngle(0.0f, 0.0f, D3DX_PI);
	wall->transform->scale = { 0.52f, 0.52f, 0.05f };
	wall->transform->position.y = 49.0f;

	wall = GameObject::CreateObuseObject("data\\OBUS\\Frame.obus");
	wall->AddComponent<Obstacle>()->SetType(Obstacle::OBSTACLETYPE::OBSTACLE_BLACK);
	wall->transform->SetEulerAngle(0.0f, D3DX_PI, 0.0f);
	wall->transform->scale = { 0.52f, 0.52f, 0.05f };
	wall->transform->position.y = 49.0f;

	wall = GameObject::CreateObuseObject("data\\OBUS\\Frame.obus");
	wall->AddComponent<Obstacle>()->SetType(Obstacle::OBSTACLETYPE::OBSTACLE_BLACK);
	wall->transform->SetEulerAngle(0.0f, D3DX_PI, D3DX_PI);
	wall->transform->scale = { 0.52f, 0.52f, 0.05f };
	wall->transform->position.y = 49.0f;

	m_stopText = new GameObject("StopText");
	m_stopText->AddComponent<TextUI>()->SetText("<color=255,0,0>動け！！");
	m_stopText->GetComponent<TextUI>()->SetFontSize(250);
	m_stopText->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
	m_stopText->transform->position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 80.0f, 0.0f };
	m_stopText->SetActive(false);
	//// プレイヤー
	//GameObject* player = new GameObject("Player");
	//player->AddComponent<CapsuleCollider>()->SetRadius(5.0f);
	//player->GetComponent<CapsuleCollider>()->SetHeight(15.0f);
	//player->AddComponent<RigidBody>()->SetMass(30.0f);
	//player->AddComponent<Player>();
	//player->transform->position.y += 10.0f;

	// カメラを作成する
	m_camera = new GameObject("Camera");
	m_camera->AddComponent<Camera>()->SetMode(Camera::MODE::MODE_PARALLEL);
	m_camera->transform->position = {0.0f, 50.0f, -500.0f };

	// BGM再生
	GameObject* bgm = new GameObject("BGM");
	bgm->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\BGM\\game_00.wav", true, true), Group::BGM);
	bgm->GetComponent<AudioSource>()->SetVolume(0.2f);
}

//=============================================================
// 更新
//=============================================================
void GameScene::Update()
{
	if (m_stopCount >= 2)
	{
		m_stopText->SetActive(true);
	}
	else
	{
		m_stopText->SetActive(false);
	}

	auto gameObjects = GameObject::GetAllGameObjects();
	for (auto obj : gameObjects)
	{
		if (obj->GetComponent<Collision>() != nullptr)
		{
			if (obj->GetComponent<Collision>()->GetCollision() != nullptr)
			{
				obj->GetComponent<Collision>()->GetCollision()->setRestitution(1.0f);
			}
		}
	}

	if (m_ball->transform->position.x <= -200.0f ||
		m_ball->transform->position.x >= 200.0f ||
		m_ball->transform->position.y <= -100.0f ||
		m_ball->transform->position.y >= 200.0f ||
		m_stopCount >= 3)
	{
		GameObject* se = new GameObject("SE", TAG_NODESTROY);
		se->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\SE\\death.mp3", false, false), Group::SFX, true);
		AppScene->SetScene<ResultScene>();
	}

	if (m_isStarted)
	{
		if (m_recordTime <= 0.0f)
		{
			auto rigidbody = m_ball->GetComponent<RigidBody>()->GetRigidBody();
			float speed = fabsf(rigidbody->getLinearVelocity().getX()) + fabsf(rigidbody->getLinearVelocity().getY());
			m_speedStack.push_back(speed);
			m_recordTime = 3.0f;

			if (speed <= 8.0f)
			{
				m_stopCount++;
			}
			else
			{
				m_stopCount = 0;
			}
		}
		else
		{
			m_recordTime -= Main::GetInstance().GetDeltaTime();
		}
	}

	// 障害物ジェネレーターの更新
	if (m_isStarted) ObstacleGenerator::GetInstance().Update();
}

void GameScene::SetStarted(const bool& value)
{
	m_isStarted = value;
	if (m_isStarted) m_startTime = timeGetTime();
}

float GameScene::GetAveSpeed()
{
	float allSpeed = 0.0f;
	for (auto val : m_speedStack)
	{
		allSpeed += val;
	}
	allSpeed /= (float)m_speedStack.size();
	return allSpeed;
}

