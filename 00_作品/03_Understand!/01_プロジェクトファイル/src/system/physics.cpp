//------------------------------------------------------------
// @file			physics.cpp
// @brief		物理
// @detail	物理演算の処理
//------------------------------------------------------------
#include "physics.h"
#include "components/3d/collision.h"

// 定数
constexpr bool DEBUG_LINE_VISIBLE = false;	// ラインを表示するか
constexpr int DEBUG_LINE_LIFE = 30;				// ラインの寿命

//=============================================================
// 初期化
//=============================================================
void Physics::Init()
{
	// コリジョンの設定
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	// ブロードフェーズ法の設定
	m_overlappingPairCache = new btDbvtBroadphase();

	// 拘束のソルバ設定
	m_solver = new btSequentialImpulseConstraintSolver;

	// Bulletワールドを作成
	m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	// ゴーストの呼び出し設定
	m_ghostPairCollback = new btGhostPairCallback();
	m_world->getPairCache()->setInternalGhostPairCallback(m_ghostPairCollback);

	// デバッグの設定
	m_debugDrawer = new DebugPhysics();
	m_debugDrawer->setDebugMode(DEBUG_LINE_VISIBLE ? btIDebugDraw::DBG_DrawWireframe : btIDebugDraw::DBG_NoDebug);
	m_world->setDebugDrawer(m_debugDrawer);

	// アクションインターフェイスの設定
	m_actionInterface = new ActionInterface();
	m_world->addAction(m_actionInterface);

	// 重力を設定
	//m_world->setGravity(btVector3(0, -90, 0));
	m_world->setGravity(btVector3(0, 0, 0));
}

//=============================================================
// 終了
//=============================================================
void Physics::Uninit()
{
	// 拘束を削除する
	for (int i = 0; i < m_world->getNumConstraints(); i++)
	{
		btTypedConstraint* pConstraint = m_world->getConstraint(i);
		m_world->removeConstraint(pConstraint);
		delete pConstraint;
	}

	// 物理ワールドの破棄
	if (m_world != nullptr)
	{
		delete m_world;
		m_world = nullptr;
	}

	// アクションインターフェイスの破棄
	if (m_actionInterface != nullptr)
	{
		delete m_actionInterface;
		m_actionInterface = nullptr;
	}

	// デバッグ描画の破棄
	if (m_debugDrawer != nullptr)
	{
		delete m_debugDrawer;
		m_debugDrawer = nullptr;
	}

	// ゴーストコールバックの破棄
	if (m_ghostPairCollback != nullptr)
	{
		delete m_ghostPairCollback;
		m_ghostPairCollback = nullptr;
	}

	// ソルバの破棄
	if (m_solver != nullptr)
	{
		delete m_solver;
		m_solver = nullptr;
	}

	// オーバーラッピングペアキャッシュの破棄
	if (m_overlappingPairCache != nullptr)
	{
		delete m_overlappingPairCache;
		m_overlappingPairCache = nullptr;
	}

	// ディスパッチャーの破棄
	if (m_dispatcher != nullptr)
	{
		delete m_dispatcher;
		m_dispatcher = nullptr;
	}

	// コリジョン設定の破棄
	if (m_collisionConfiguration != nullptr)
	{
		delete m_collisionConfiguration;
		m_collisionConfiguration = nullptr;
	}
}

//=============================================================
// 更新
//=============================================================
void Physics::Update()
{
	// 物理ワールドを更新する
	m_world->stepSimulation(static_cast<btScalar>(Main::GetInstance().GetDeltaTime()), 1);
	m_world->debugDrawWorld();

	// 新しく追加されたコリジョンを登録する
	for (auto itr = m_collisions.begin(); itr != m_collisions.end(); itr++)
	{
		Collision* collision = *itr;
		RigidBody* rigidbody = collision->gameObject->GetComponent<RigidBody>();

		// トランスフォームが変更されていた場合
		bool isChanged = false;
		if ((collision->gameObject->transform->GetWorldPosition() != collision->GetOldTransform().position ||
			collision->gameObject->transform->GetWorldRotation() != collision->GetOldTransform().rotation ||
			collision->gameObject->transform->GetWorldScale() != collision->GetOldTransform().scale) &&
			collision->GetCollision() != nullptr)
		{
			// 位置を設定
			btTransform changedTransform;
			D3DXVECTOR3 worldPos = collision->gameObject->transform->GetWorldPosition();
			changedTransform.setIdentity();
			changedTransform.setOrigin(btVector3(worldPos.x, worldPos.y, worldPos.z));

			// 回転を設定
			D3DXQUATERNION worldRot = collision->gameObject->transform->GetWorldRotation();
			btQuaternion quaternion(worldRot.x, worldRot.y, worldRot.z, worldRot.w);
			changedTransform.setRotation(quaternion);

			// スケールが変更されているか
			if (collision->gameObject->transform->GetWorldScale() != collision->GetOldTransform().GetWorldScale())
			{
				collision->GetUpdateFlag() = true;
			}


			// リジッドボディの場合はアクティブにする
			if (rigidbody != nullptr)
			{
				rigidbody->GetRigidBody()->activate(true);
			}

			// 変更を適用する
			collision->GetCollision()->setWorldTransform(changedTransform);
			isChanged = true;
		}
		
		if (rigidbody != nullptr && collision->GetCollision() != nullptr)
		{ // リジッドボディのトランスフォームを更新する
			// 結果を取得する
			btTransform result;
			rigidbody->GetRigidBody()->getMotionState()->getWorldTransform(result);

			// トランスフォームに変更があったとき
			if (isChanged)
			{
				result = collision->GetCollision()->getWorldTransform();
			}

			// 更新する
			collision->transform->position = { result.getOrigin().getX(), result.getOrigin().getY(), result.getOrigin().getZ() };
			collision->transform->rotation = { result.getRotation().getX(), result.getRotation().getY(), result.getRotation().getZ(), result.getRotation().getW() };
		}

		// 前回のトランスフォーム
		collision->UpdateOldTransform();

		// 更新フラグが立っているとき
		if (collision->GetUpdateFlag())
		{
			// 構成する
			collision->Build();
		}
	}
}

//=============================================================
// コリジョンを追加する
//=============================================================
void Physics::AddCollision(Collision* collision)
{
	// 重複していないときのみ追加する
	if (std::find(m_collisions.begin(), m_collisions.end(), collision) == m_collisions.end())
	{
		m_collisions.emplace_back(collision);
	}
}

//=============================================================
// コリジョンを削除する
//=============================================================
void Physics::RemoveCollision(Collision* collision)
{
	auto target = std::find(m_collisions.begin(), m_collisions.end(), collision);
	if (target != m_collisions.end())
	{
		m_collisions.erase(target);
	}
}

//=============================================================
// btCollisionObjectからCollisionを探す
//=============================================================
Collision* Physics::FindCollision(const btCollisionObject* collisionObject)
{
	for (auto itr = m_collisions.begin(); itr != m_collisions.end(); itr++)
	{
		if ((*itr)->GetCollision() == collisionObject)
		{
			return *itr;
		}
	}
	return nullptr;
}


//=============================================================
// デバッグ : 線の描画
//=============================================================
void Physics::DebugPhysics::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Gizmo.DrawLine({ from.getX(), from.getY(), from.getZ() }, { to.getX(), to.getY(), to.getZ() }, { color.getX(), color.getY(), color.getZ(), 1.0f });
}


//=============================================================
// アクションインターフェイス
//=============================================================
void ActionInterface::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	// コリジョンを取得する
	std::vector<Collision*>& collisions = Manager::GetInstance()->GetPhysics()->GetCollisions();
	for (auto itr = collisions.begin(); itr != collisions.end(); itr++)
	{
		if ((*itr)->GetCollision() == nullptr)
		{
			continue;
		}

		// コリジョン情報を取得する
		Collision* collision = *itr;																												// コリジョン
		RigidBody* rigidbody = collision->gameObject->GetComponent<RigidBody>();						// リジッドボディ（無い場合は nullptr）
		bool isTrigger = collision->GetTrigger();																						// トリガーか
		std::vector<Collision*>& overlappingCollisions = collision->GetOverlappingCollisions();			// 重なっているコリジョン
		std::vector<Component*>& componentList = collision->gameObject->GetComponents();	// コンポーネントリスト

		if (rigidbody == nullptr)
		{ // ゴーストオブジェクト

			btGhostObject* ghostObj = (btGhostObject*)collision->GetCollision();

			// 現在、重なっているコリジョンをリスト化する
			int overlappingNum = ghostObj->getNumOverlappingObjects();
			std::vector<Collision*> currentOverlapping;
			for (int i = 0; i < overlappingNum; i++)
			{
				// コリジョンを取得する
				Collision* overlapObj = Manager::GetInstance()->GetPhysics()->FindCollision(ghostObj->getOverlappingObject(i));
				if (overlapObj != nullptr)
				{
					currentOverlapping.emplace_back(overlapObj);
				}
			}

			// Enterイベント
			for (auto compItr = componentList.begin(); compItr != componentList.end(); compItr++)
			{
				for (auto overlapItr = currentOverlapping.begin(); overlapItr != currentOverlapping.end(); overlapItr++)
				{
					if (std::find(overlappingCollisions.begin(), overlappingCollisions.end(), *overlapItr) == overlappingCollisions.end())
					{
						isTrigger ? (*compItr)->OnTriggerEnter(*overlapItr) : (*compItr)->OnCollisionEnter(*overlapItr);
					}
				}
			}

			// Stayイベント
			for (auto compItr = componentList.begin(); compItr != componentList.end(); compItr++)
			{
				for (auto overlapItr = currentOverlapping.begin(); overlapItr != currentOverlapping.end(); overlapItr++)
				{
					isTrigger ? (*compItr)->OnTriggerStay(*overlapItr) : (*compItr)->OnCollisionStay(*overlapItr);
				}
			}

			// Exitイベント
			for (auto compItr = componentList.begin(); compItr != componentList.end(); compItr++)
			{
				for (auto overlapItr = overlappingCollisions.begin(); overlapItr != overlappingCollisions.end(); overlapItr++)
				{
					if (std::find(currentOverlapping.begin(), currentOverlapping.end(), *overlapItr) == currentOverlapping.end())
					{
						isTrigger ? (*compItr)->OnTriggerExit(*overlapItr) : (*compItr)->OnCollisionExit(*overlapItr);
					}
				}
			}

			// 重なっているコリジョンを更新する
			overlappingCollisions.clear();
			overlappingCollisions = std::move(currentOverlapping);
		}


	}
}
