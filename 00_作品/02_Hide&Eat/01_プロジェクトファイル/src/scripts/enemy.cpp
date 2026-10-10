//=============================================================
//
// 敵 [enemy.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "enemy.h"
#include "internal/physics.h"
#include "component/3d/line.h"
#include "scripts/player.h"
#include "scene/game.h"
#include "manager.h"
#include "component/other/sound.h"

const float Enemy::DETECTION_DISTANCE = -150.0f;
const float Enemy::FOLLOW_RANGE = 200.0f;
//const float Enemy::FOLLOW_TIME = 5.0f;
const float Enemy::MOVE_SPEED = -0.5f;
//=============================================================
// [Enemy] 初期化
//=============================================================
void Enemy::Init()
{
	// プレイヤーを取得する
	m_player = GameObject::Find("Player");

	//	デバッグラインを生成する
	for (int i = 0; i < 3; i++)
	{
		m_line[i] = new GameObject();
		m_line[i]->AddComponent<CLine>();
	}

	// プレイヤーヒットコリジョンの作成
	GameObject* hitPlayerObj = new GameObject();
	hitPlayerObj->SetParent(gameObject);
	hitPlayerObj->AddComponent<CCapsuleCollider>(13.0f, 24.5f, D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_hitPlayer = CCollision::GetCollision(hitPlayerObj);
	m_hitPlayer->IsTrigger(true);
}

//=============================================================
// [Enemy] 更新
//=============================================================
void Enemy::Update()
{
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->IsGameOver() ||
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->IsClear())
		return;

	// レイを更新する
	UpdateRay();

	// 移動する
	UpdateMove();

	// プレイヤーのヒット判定
	if (!(m_player->GetComponent<PlayerControl>()->GetIsHide() && m_state == STATE::PATROL))
	{
		std::vector<GameObject*> overlapping = m_hitPlayer->GetOverlappingGameObjects();
		for (auto itr = overlapping.begin(); itr != overlapping.end(); itr++)
		{
			if ((*itr)->GetName() == "Player")
			{ // プレイヤーの場合
				if (m_player->GetComponent<PlayerControl>()->GetLevel() >= 8)
				{ // ゲームクリア
					static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->onClear();
					gameObject->Destroy();

					GameObject* sound = new GameObject;
					sound->AddComponent<CSound>()->LoadWav("data\\SE\\eat_00.wav");
					sound->GetComponent<CSound>()->IsStoppedDestroy();
					sound->GetComponent<CSound>()->Play();
				}
				else
				{ // ゲームオーバー
					static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->onGameOver();
				}
				break;
			}
		}
	}

	//// 追従タイマー
	//if (m_detectionTimer > 0.0f)
	//{
	//	m_detectionTimer -= CManager::GetInstance()->GetDeltaTime();
	//}

	// 移動タイマー
	//if (m_moveTimer > 0.0f)
	//{
	//	m_moveTimer -= CManager::GetInstance()->GetDeltaTime();
	//}
}

//=============================================================
// [Enemy] レイを更新する
//=============================================================
void Enemy::UpdateRay()
{
	m_line[0]->SetVisible(!m_player->GetComponent<PlayerControl>()->GetIsHide());
	m_line[1]->SetVisible(!m_player->GetComponent<PlayerControl>()->GetIsHide());
	m_line[2]->SetVisible(!m_player->GetComponent<PlayerControl>()->GetIsHide());

	// ハイドしているときはレイを飛ばさない
	if (m_player->GetComponent<PlayerControl>()->GetIsHide())
	{
		return;
	}

	// レイの座標
	btVector3 Start;
	btVector3 End;

	STATE oldState = m_state;

	// 1
	Start = btVector3(transform->GetWPos().x, transform->GetWPos().y + 10.0f, transform->GetWPos().z);
	End = Start + btVector3(sinf(transform->GetWRot().y) * DETECTION_DISTANCE, 0.0f, cosf(transform->GetWRot().y) * DETECTION_DISTANCE);
	m_line[0]->GetComponent<CLine>()->SetLine(D3DXVECTOR3(Start.getX(), Start.getY(), Start.getZ()), D3DXVECTOR3(End.getX(), End.getY(), End.getZ()));

	btCollisionWorld::ClosestRayResultCallback RayCallback0(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback0);
	if (RayCallback0.hasHit() && CCollision::GetGameObjectFromCollisionObj(RayCallback0.m_collisionObject)->GetName() == "Player")
	{ // プレイヤーにヒットしたとき
		m_state = STATE::FOLLOW;
	}

	// 2
	Start = btVector3(transform->GetWPos().x, transform->GetWPos().y + 10.0f, transform->GetWPos().z);
	End = Start + btVector3(sinf(transform->GetWRot().y - 0.3f) * DETECTION_DISTANCE, 0.0f, cosf(transform->GetWRot().y - 0.3f) * DETECTION_DISTANCE);
	m_line[1]->GetComponent<CLine>()->SetLine(D3DXVECTOR3(Start.getX(), Start.getY(), Start.getZ()), D3DXVECTOR3(End.getX(), End.getY(), End.getZ()));

	btCollisionWorld::ClosestRayResultCallback RayCallback1(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback1);
	if (RayCallback1.hasHit() && CCollision::GetGameObjectFromCollisionObj(RayCallback1.m_collisionObject)->GetName() == "Player")
	{ // プレイヤーにヒットしたとき
		m_state = STATE::FOLLOW;
	}

	// 3
	Start = btVector3(transform->GetWPos().x, transform->GetWPos().y + 10.0f, transform->GetWPos().z);
	End = Start + btVector3(sinf(transform->GetWRot().y + 0.3f) * DETECTION_DISTANCE, 0.0f, cosf(transform->GetWRot().y + 0.3f) * DETECTION_DISTANCE);
	m_line[2]->GetComponent<CLine>()->SetLine(D3DXVECTOR3(Start.getX(), Start.getY(), Start.getZ()), D3DXVECTOR3(End.getX(), End.getY(), End.getZ()));

	btCollisionWorld::ClosestRayResultCallback RayCallback2(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback2);
	if (RayCallback2.hasHit() && CCollision::GetGameObjectFromCollisionObj(RayCallback2.m_collisionObject)->GetName() == "Player")
	{ // プレイヤーにヒットしたとき
		m_state = STATE::FOLLOW;
	}

	if (oldState != m_state && m_state == STATE::FOLLOW)
	{
		GameObject* sound = new GameObject;
		sound->AddComponent<CSound>()->LoadWav("data\\SE\\find_00.wav");
		sound->GetComponent<CSound>()->IsStoppedDestroy();
		sound->GetComponent<CSound>()->Play();
	}
}

//=============================================================
// [Enemy] 移動する
//=============================================================
void Enemy::UpdateMove()
{
	float rot;
	switch (m_state)
	{
	case Enemy::PATROL:
		rot = m_patrolAngle * 0.02f;
		transform->Rotate(0.0f, rot, 0.0f);
		transform->Translate(sinf(transform->GetWRot().y) * MOVE_SPEED, 0.0f, cosf(transform->GetWRot().y) * MOVE_SPEED);
		m_patrolAngle -= rot;

		// 回転処理
		m_patrolRotationTimer -= CManager::GetInstance()->GetDeltaTime();
		if (m_patrolRotationTimer <= 0.0f)
		{
			// 向きをランダムで決める
			m_patrolAngle += Benlib::RandomFloat(-D3DX_PI, D3DX_PI);

			// 次の回転タイマーを設定する
			m_patrolRotationTimer = Benlib::RandomFloat(1.0f, 6.0f);
		}

		// フィールド外に出たとき
		if (fabsf(transform->GetWPos().x) > 200.0f || fabsf(transform->GetWPos().z) > 200.0f)
		{
			transform->Rotate(0.0f, (transform->GetRot().y - D3DX_PI - transform->GetRot().y) * 0.02f, 0.0f);

			// 向きをランダムで決める
			m_patrolAngle = 0.0f;

			// 次の回転タイマーを設定する
			m_patrolRotationTimer = 3.0f;
		}

		break;
	case Enemy::FOLLOW:
		transform->SetRot(0.0f, Benlib::PosAngle(transform->GetWPos(), m_player->transform->GetWPos()) + D3DX_PI, 0.0f);
		transform->Translate(sinf(transform->GetWRot().y) * MOVE_SPEED, 0.0f, cosf(transform->GetWRot().y) * MOVE_SPEED);

		if (Benlib::PosPlaneDistance(transform->GetWPos(), m_player->transform->GetWPos()) >= FOLLOW_RANGE)
		{ // 範囲外に行ったとき
			m_state = STATE::PATROL;
		}
		break;
	}


	//if (m_detectionTimer > 0.0f)
	//{ // 追従
	//	transform->SetRot(0.0f, Benlib::PosAngle(transform->GetWPos(), m_player->transform->GetWPos()) + D3DX_PI, 0.0f);
	//}
	//else
	//{ // 追従無し

	//}

	//// 移動する
	//if (m_moveTimer > 0.0f)
	//{
	//	transform->Translate(sinf(transform->GetWRot().y) * MOVE_SPEED, 0.0f, cosf(transform->GetWRot().y) * MOVE_SPEED);
	//}
}