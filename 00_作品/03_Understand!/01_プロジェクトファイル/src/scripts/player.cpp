//------------------------------------------------------------
// @file		player.cpp
// @brief	プレイヤーそのものです。
//------------------------------------------------------------
#include "player.h"
#include "components/3d/camera.h"
#include "scenes/game.h"

//=============================================================
// 初期化
//=============================================================
void Player::Init()
{
	// リジッドボディのオプション
	gameObject->GetComponent<RigidBody>()->SetAlwayActive(true);
}

//=============================================================
// 終了
//=============================================================
void Player::Uninit()
{
}

//=============================================================
// 更新
//=============================================================
void Player::Update()
{
	gameObject->GetComponent<RigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));

	// 操作
	Controller();

	// カメラ操作
	CameraControl();

	// 接地状態更新
	UpdateLanding();
}

//=============================================================
// 操作系
//=============================================================
void Player::Controller()
{
	D3DXVECTOR3  MoveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スティックの数値を取得する
	D3DXVECTOR2 stickL = AppInput->GetLStickValue();

	if (stickL == D3DXVECTOR2(0.0f, 0.0f))
	{ // ボタン入力
		if (AppInput->GetKey(KeyCode::W) || AppInput->GetKey(KeyCode::PadUp))
		{
			MoveVec.z += 1.0f;
		}
		if (AppInput->GetKey(KeyCode::A) || AppInput->GetKey(KeyCode::PadLeft))
		{
			MoveVec.x += -1.0f;
		}
		if (AppInput->GetKey(KeyCode::S) || AppInput->GetKey(KeyCode::PadDown))
		{
			MoveVec.z += -1.0f;
		}
		if (AppInput->GetKey(KeyCode::D) || AppInput->GetKey(KeyCode::PadRight))
		{
			MoveVec.x += 1.0f;
		}
	}
	else
	{ // スティック入力
		MoveVec = {stickL.x, 0.0f, stickL.y};
	}

	if (MoveVec != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		// 進行方向を計算する
		float angle = Benlib::PosAngle({ 0.0f, 0.0f, 0.0f }, MoveVec);
		angle += m_yaw;

		gameObject->GetComponent<RigidBody>()->GetRigidBody()->setLinearVelocity(btVector3(
			sinf(angle) * MOVE_SPEED,
			gameObject->GetComponent<RigidBody>()->GetRigidBody()->getLinearVelocity().getY(),
			cosf(angle) * MOVE_SPEED
		));
	}
}
//=============================================================
// 主観視点におけるカメラ操作系
//=============================================================
void Player::CameraControl()
{
	// アクティブでないときはスキップする
	if (!Main::GetInstance().IsActiveWindow()) return;

	// 回転量を計算する
	D3DXVECTOR2 moveCursor = Main::GetInstance().GetMoveCursor();
	D3DXVECTOR2 moveStick = AppInput->GetRStickValue() * 0.0001f;
	moveStick.y *= ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH);
	
	// マウス
	m_yaw += (moveCursor.x) * 0.001f;
	m_pitch += (moveCursor.y) * 0.001f;

	// コントローラー
	m_yaw += (moveStick.x) * 0.01f;
	m_pitch += (moveStick.y * -1.0f) * 0.01f;

	// 上下の制限
	m_pitch = std::clamp<float>(m_pitch, -D3DX_PI * 0.49f, D3DX_PI * 0.49f);

	// 左右のリセット
	if (m_yaw < -D3DX_PI * 2.0f)
	{
		m_yaw += D3DX_PI * 2.0f;
	}
	else if (m_yaw > D3DX_PI * 2.0f)
	{
		m_yaw -= D3DX_PI * 2.0f;
	}

	// カーソルを中心に戻す
	Main::GetInstance().SetCursorClientPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// 向きを変更する
	Manager::GetInstance()->GetScene<GameScene>()->GetCamera()->transform->SetEulerAngle(m_pitch, m_yaw, 0.0f);

	// 向きを変更する
	//gameObject->FindChildrenByName("model")->transform->SetEulerAngle(
	//	-m_pitch,                      // ← pitch（上下）を反映
	//	m_yaw + D3DX_PI,              // yaw（左右）＋180度
	//	0.0f
	//);
}

//=============================================================
// 接地状態を更新する
//=============================================================
void Player::UpdateLanding()
{
	auto rigidbody = gameObject->GetComponent<RigidBody>()->GetRigidBody();
	btVector3 vel = rigidbody->getLinearVelocity();

	// プレイヤー位置から下方向にレイを飛ばす
	btVector3 from = Benlib::ToBtVector3(transform->GetWorldPosition());
	btVector3 to = from - btVector3(0, 50.0f, 0);

	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
	AppPhysics->GetWorld()->rayTest(from, to, rayCallback);

	float playerHeight = 23.0f;
	if (rayCallback.hasHit())
	{
		float dist = (from - rayCallback.m_hitPointWorld).length();

		// しきい値以下なら地面に押し付ける
		if (dist > playerHeight)
		{
			transform->position.y = rayCallback.m_hitPointWorld.getY() + (playerHeight - 3.0f);
			rigidbody->setLinearVelocity(btVector3(vel.x(), 0, vel.z()));
		}
	}
}
