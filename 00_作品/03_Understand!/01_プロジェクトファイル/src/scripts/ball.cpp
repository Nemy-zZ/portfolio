//------------------------------------------------------------
// @file		ball.h
// @brief	ボール（プレイヤー）
//------------------------------------------------------------
#include "ball.h"
#include "components/3d/model.h"
#include "scenes/game.h"
#include "components/2d/sprite.h"
#include "components/3d/billboard.h"

#include "components/2d/text.h"

//=============================================================
// 初期化
//=============================================================
void Ball::Init()
{
	m_type = BALLTYPE::BALL_WHITE;
	m_power = 1.0f;

	// 矢印
	m_arrow = new GameObject("Arrow");
	m_arrow->transform->size = { 200.0f, 200.0f };
	m_arrow->AddComponent<Sprite>()->SetTexture("data\\TEXTURE\\arrow.png");
	m_arrow->GetComponent<Sprite>()->SetAnchorPoint({ 100.0f, 150.0f });
}

//=============================================================
// 更新
//=============================================================
void Ball::Update()
{
	if (!AppScene->GetScene<GameScene>()->IsStarted())
	{
		StartingUpdate();
		return;
	}

	// モデルの取得
	GameObject* modelObj = gameObject->FindChildrenByName("model");
	auto model = modelObj->GetComponent<Model>();
	if (model != nullptr)
	{
		model->SetColorEnabled(true);
		model->SetColorBlack(m_type == BALLTYPE::BALL_BLACK);
	}

	// 色切り替え
	if (AppInput->GetKeyDown(KeyCode::B) || AppInput->GetKeyDown(KeyCode::PadB))
	{
		SetType(GetType() == BALLTYPE::BALL_WHITE ? BALLTYPE::BALL_BLACK : BALLTYPE::BALL_WHITE);
	}

	// 方向転換
	auto rigidbody = gameObject->GetComponent<RigidBody>()->GetRigidBody();

	// 物理オプション
	rigidbody->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
	rigidbody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

	D3DXVECTOR2 stickL = AppInput->GetLStickValue();

	if (stickL != D3DXVECTOR2(0.0f, 0.0f))
	{
		// 進行方向を計算する
		m_angle = atan2f(stickL.x, stickL.y);
		rigidbody->applyCentralForce(btVector3(sinf(m_angle + D3DX_PI) * -BASE_SPEED, cosf(m_angle + D3DX_PI) * -BASE_SPEED, 0.0f));
	}
	
	//rigidbody->setLinearVelocity(rigidbody->getLinearVelocity() * m_power);
	//rigidbody->applyCentralForce(btVector3(sinf(m_angle + D3DX_PI) * -BASE_SPEED * m_power, cosf(m_angle + D3DX_PI) * -BASE_SPEED * m_power, 0.0f));

	if (m_timer <= 0.0f)
	{
		m_power += POWER;
		rigidbody->setLinearVelocity(rigidbody->getLinearVelocity() * m_power);

		m_timer = POWERUP_TIME;
	}
	else
	{
		m_timer -= Main::GetInstance().GetDeltaTime();
	}

	//if (AppScene->GetScene<GameScene>()->IsStarted() &&
	//	gameObject->GetComponent<RigidBody>()->GetRigidBody()->getLinearVelocity() == btVector3(0.0f, 0.0f, 0.0f))
	//{
	//	gameObject->GetComponent<RigidBody>()->GetRigidBody()->applyCentralForce(
	//		btVector3(sinf(m_arrowAngle + D3DX_PI) * -100.0f, cosf(m_arrowAngle + D3DX_PI) * -100.0f, 0.0f)
	//	);
	//	//AppPhysics->GetWorld()->stepSimulation(static_cast<btScalar>(Main::GetInstance().GetDeltaTime()), 1);
	//}

}

//=============================================================
// 壁に当たったとき
//=============================================================
void Ball::OnCollisionEnter(Collision* other)
{
	//m_power *= 1.5f;
	//Log::SendLog("Yeeeeee");
}

//=============================================================
// 回転
//=============================================================
void Ball::StartingUpdate()
{
	m_arrowAngle += 0.05f;
	m_arrow->transform->SetEulerAngle(0.0f, 0.0f, m_arrowAngle);
	m_arrow->transform->position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

	if (m_arrowAngle >= D3DX_PI * 2.0f)
	{
		m_arrowAngle -= D3DX_PI * 2.0f;
	}

	auto scene = AppScene->GetScene<GameScene>();
	if (!scene->IsStarted() && AppInput->GetKeyDown(KeyCode::PadA))
	{
		gameObject->GetComponent<RigidBody>()->GetRigidBody()->setLinearVelocity(
			btVector3(sinf(m_arrowAngle) * 200.0f, cosf(m_arrowAngle) * 200.0f, 0.0f)
		);
		scene->SetStarted(true);

		GameObject* se = new GameObject("SE", TAG_NODESTROY);
		se->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\SE\\shot.mp3", false, false), Group::SFX, true);

		Destroy(m_arrow);
	}
}
