//------------------------------------------------------------
// @file			collision.cpp
// @brief		コリジョン
// @detail	当たり判定
//------------------------------------------------------------
#include "collision.h"

//=============================================================
// 初期化
//=============================================================
void Collision::Init()
{
	// すでにコリジョンコンポーネントが含まれているとき
	if (static_cast<int>(gameObject->GetComponents<Collision>().size()) > 1)
	{
		Destroy(this);
		return;
	}

	// トランスフォーム情報を更新する
	m_oldTransform = *transform;

	// 物理リストに追加する
	Manager::GetInstance()->GetPhysics()->AddCollision(this);
}

//=============================================================
// 終了
//=============================================================
void Collision::Uninit()
{
	// 物理リストから削除する
	Manager::GetInstance()->GetPhysics()->RemoveCollision(this);

	// 形状を破棄する
	if (m_shape != nullptr)
	{
		delete m_shape;
		m_shape = nullptr;
	}

	// コリジョンを破棄する
	if (m_collision != nullptr)
	{
		Manager::GetInstance()->GetPhysics()->GetWorld()->removeCollisionObject(m_collision);
		delete m_collision;
		m_collision = nullptr;
	}

	// 副次的なコンポーネントを削除する
	if (gameObject != nullptr)
	{
		auto rigidBody = gameObject->GetComponent<RigidBody>();
		if (rigidBody != nullptr)
		{
			Destroy(rigidBody);
		}
	}
}

//=============================================================
// 更新
//=============================================================
void Collision::Update()
{
	
}

//=============================================================
// 前回のトランスフォームを保存する
//=============================================================
void Collision::UpdateOldTransform()
{
	m_oldTransform.position = transform->GetWorldPosition();
	m_oldTransform.rotation = transform->GetWorldRotation();
	m_oldTransform.scale = transform->GetWorldScale();
}

//=============================================================
// ビルドする
//=============================================================
void Collision::Build()
{
	// リジッドボディを取得する
	auto rigidbody = gameObject->GetComponent<RigidBody>();

	// ベロシティーを保存しておく
	btVector3 linerVelocity = btVector3(0.0f, 0.0f, 0.0f);
	btVector3 angularVelocity = btVector3(0.0f, 0.0f, 0.0f);
	if (rigidbody != nullptr)
	{
		btRigidBody* coreRigidbody = rigidbody->GetRigidBody();
		if (coreRigidbody != nullptr)
		{
			linerVelocity = rigidbody->GetRigidBody()->getLinearVelocity();
			angularVelocity = rigidbody->GetRigidBody()->getAngularVelocity();
		}
	}

	// コリジョンを破棄する
	if (m_collision != nullptr)
	{
		Manager::GetInstance()->GetPhysics()->GetWorld()->removeCollisionObject(m_collision);
		delete m_collision;
		m_collision = nullptr;
	}

	// コライダーシェイプを生成する
	GenerateColliderShape();

	// 位置を設定
	btTransform startTransform;
	D3DXVECTOR3 worldPos = transform->GetWorldPosition();
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(worldPos.x, worldPos.y, worldPos.z));

	// 回転を設定
	D3DXQUATERNION worldRot = transform->GetWorldRotation();
	btQuaternion quaternion(worldRot.x, worldRot.y, worldRot.z, worldRot.w);
	startTransform.setRotation(quaternion);

	if (rigidbody != nullptr)
	{ // リジッドボディを作成する
		// 質量
		btScalar mass = rigidbody->GetMass();

		// 移動するオブジェクトか
		bool isDynamic = (mass != 0.0f);

		// 慣性モーメント
		btVector3 inertia(0, 0, 0);
		if (isDynamic)
		{
			m_shape->calculateLocalInertia(mass, inertia);
		}

		// 剛体操作
		rigidbody->SetMotionState(new btDefaultMotionState(startTransform));

		// 剛体作成
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, rigidbody->GetMotionState(), m_shape, inertia);
		m_collision = new btRigidBody(rb_info);

		// 摩擦設定
		m_collision->setFriction(m_friction);

		// ベロシティを引き継ぐ
		btRigidBody::upcast(m_collision)->setLinearVelocity(linerVelocity);
		btRigidBody::upcast(m_collision)->setAngularVelocity(angularVelocity);

		// 物理ワールドに追加する
		Manager::GetInstance()->GetPhysics()->GetWorld()->addRigidBody((btRigidBody*)m_collision);
	}
	else
	{ // ゴーストオブジェクトを作成する
		m_collision = new btGhostObject;

		// 形状を設定する
		m_collision->setCollisionShape(m_shape);

		// 摩擦設定
		m_collision->setFriction(m_friction);

		// 他のオブジェクトに干渉するか
		if (m_trigger)
		{
			m_collision->setCollisionFlags(m_collision->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		// トランスフォームを設定する
		m_collision->setWorldTransform(startTransform);

		// 物理ワールドに追加する
		Manager::GetInstance()->GetPhysics()->GetWorld()->addCollisionObject(m_collision);
	}

	// フラグを更新する
	m_updateFlag = false;
}

//=============================================================
// トリガーの設定
//=============================================================
void Collision::SetTrigger(const bool& enabled)
{
	if (m_trigger != enabled)
	{
		m_trigger = enabled;
		m_updateFlag = true;
	}
}

//=============================================================
// 摩擦を設定する
//=============================================================
void Collision::SetFriction(const float& friction)
{
	if (m_friction != friction)
	{
		m_friction = friction;
		m_updateFlag = true;
	}
}

//=============================================================
// コライダーシェイプを生成する
//=============================================================
void Collision::GenerateColliderShape()
{
	// コライダーを取得する
	std::vector<Collider*> colliders = gameObject->GetComponents<Collider>(true);

	// 現在の形状を破棄する
	if (m_shape != nullptr)
	{
		// コライダーの終了処理
		for (auto itr = colliders.begin(); itr != colliders.end(); itr++)
		{
			(*itr)->Uninit();
		}

		// 削除
		delete m_shape;
		m_shape = nullptr;
	}

	// シェイプを作成する
	m_shape = new btCompoundShape;

	// 形状を追加
	for (auto itr = colliders.begin(); itr != colliders.end(); itr++)
	{
		(*itr)->AddShape(m_shape);
	}

	// スケールを調整する
	m_shape->setLocalScaling(btVector3(transform->GetWorldScale().x, transform->GetWorldScale().y, transform->GetWorldScale().z));
}

//=============================================================
// 初期化
//=============================================================
void RigidBody::Init()
{
	// すでにリジッドボディコンポーネントが含まれているとき
	if (static_cast<int>(gameObject->GetComponents<RigidBody>().size()) > 1)
	{
		Destroy(this);
		return;
	}

	// コリジョンコンポーネントが含まれていないとき
	if (gameObject->GetComponents<Collision>().empty())
	{
		Destroy(this);
		return;
	}

	// コリジョンを構築する
	gameObject->GetComponent<Collision>()->UpdateOldTransform();
	gameObject->GetComponent<Collision>()->Build();
}

//=============================================================
// 終了
//=============================================================
void RigidBody::Uninit()
{
	// 更新対象に追加する
	if (gameObject != nullptr && gameObject->GetComponent<Collision>() != nullptr)
	{
		gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
	}

	// モーションステートを破棄する
	if (m_motionState != nullptr)
	{
		delete m_motionState;
		m_motionState = nullptr;
	}
}

//=============================================================
// 更新
//=============================================================
void RigidBody::Update()
{
	auto rigidbody = GetRigidBody();
	if (rigidbody != nullptr)
	{
		// 常にアクティブオプションが有効なのに設定されていない場合
		if (m_isAlwayActive && rigidbody->getActivationState() != DISABLE_DEACTIVATION)
		{
			rigidbody->setActivationState(DISABLE_DEACTIVATION);
		}

		// 常にアクティブオプションが無効なのに設定されていない場合
		if (!m_isAlwayActive && rigidbody->getActivationState() == DISABLE_DEACTIVATION)
		{
			rigidbody->setActivationState(WANTS_DEACTIVATION);
		}
	}
}

//=============================================================
// リジットボディを取得する
//=============================================================
btRigidBody* RigidBody::GetRigidBody()
{
	Collision* collision = gameObject->GetComponent<Collision>();
	if (collision != nullptr && collision->GetCollision() != nullptr)
	{
		return btRigidBody::upcast(collision->GetCollision());
	}
	return nullptr;
}

//=============================================================
// 質量を設定する
//=============================================================
void RigidBody::SetMass(const float& mass)
{
	Collision* collision = gameObject->GetComponent<Collision>();
	if (collision != nullptr && m_mass != mass)
	{
		m_mass = mass;
		collision->GetUpdateFlag() = true;
	}
}

//=============================================================
// 常にアクティブを 有効/無効 にする
//=============================================================
void RigidBody::SetAlwayActive(const bool& enabled)
{
	m_isAlwayActive = enabled;
}

//=============================================================
// モーションステートを設定する
//=============================================================
void RigidBody::SetMotionState(btMotionState* motionState)
{
	if (m_motionState != nullptr)
	{
		delete m_motionState;
		m_motionState = nullptr;
	}

	m_motionState = motionState;
}
