//------------------------------------------------------------
// @file			collider.cpp
// @brief		コライダー
// @detail	当たり判定の形状
//------------------------------------------------------------
#include "collider.h"
#include "collision.h"

//=============================================================
// コンストラクタ
//=============================================================
Collider::Collider() :
	m_offset(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),
	m_shape(nullptr)
{
}

//=============================================================
// 初期化
//=============================================================
void Collider::Init()
{
	// コリジョンコンポーネントを自動追加する
	if (gameObject->GetComponents<Collision>().empty())
	{
		gameObject->AddComponent<Collision>();
	}
}

//=============================================================
// 終了
//=============================================================
void Collider::Uninit()
{
	// シェイプを破棄する
	if (m_shape != nullptr)
	{
		if (gameObject != nullptr)
		{
			btCompoundShape* shape = gameObject->GetComponent<Collision>()->GetShape();
			if (shape != nullptr)
			{
				shape->removeChildShape(m_shape);
			}
		}

		// 削除
		delete m_shape;
		m_shape = nullptr;
	}
}

//=============================================================
// ローカル姿勢情報をトランスフォームとして取得します
//=============================================================
btTransform Collider::GetLocalTransform()
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(m_offset.x, m_offset.y, m_offset.z));

	// 回転
	btQuaternion quaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
	localTrans.setRotation(quaternion);

	return localTrans;
}


//=============================================================
// ボックスコライダー
//=============================================================
void BoxCollider::AddShape(btCompoundShape* shapes)
{
	Collider::Uninit();

	// シェイプを追加する
	m_shape = new btBoxShape(btVector3(m_size.x, m_size.y, m_size.z));
	shapes->addChildShape(GetLocalTransform(), m_shape);
}

//=============================================================
// ボックスコライダー : サイズ変更
//=============================================================
void BoxCollider::SetSize(const D3DXVECTOR3& size)
{
	m_size = size;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// スフィアコライダー
//=============================================================
void SphereCollider::AddShape(btCompoundShape* shapes)
{
	Collider::Uninit();

	// シェイプを追加する
	m_shape = new btSphereShape(m_radius);
	shapes->addChildShape(GetLocalTransform(), m_shape);
}

//=============================================================
// スフィアコライダー : 半径変更
//=============================================================
void SphereCollider::SetRadius(const float& radius)
{
	m_radius = radius;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// カプセルコライダー
//=============================================================
void CapsuleCollider::AddShape(btCompoundShape* shapes)
{
	Collider::Uninit();

	// シェイプを追加する
	m_shape = new btCapsuleShape(m_radius, m_height);
	shapes->addChildShape(GetLocalTransform(), m_shape);
}

//=============================================================
// カプセルコライダー : 半径変更
//=============================================================
void CapsuleCollider::SetRadius(const float& radius)
{
	m_radius = radius;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// カプセルコライダー : 高さ変更
//=============================================================
void CapsuleCollider::SetHeight(const float& height)
{
	m_height = height;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// 円柱コライダー
//=============================================================
void CylinderCollider::AddShape(btCompoundShape* shapes)
{
	Collider::Uninit();

	// シェイプを追加する
	m_shape = new btCylinderShape(btVector3(m_radius, m_height, m_radius));
	shapes->addChildShape(GetLocalTransform(), m_shape);
}

//=============================================================
// 円柱コライダー : 半径変更
//=============================================================
void CylinderCollider::SetRadius(const float& radius)
{
	m_radius = radius;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// 円柱コライダー : 高さ変更
//=============================================================
void CylinderCollider::SetHeight(const float& height)
{
	m_height = height;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// 円錐コライダー
//=============================================================
void ConeCollider::AddShape(btCompoundShape* shapes)
{
	Collider::Uninit();

	// シェイプを追加する
	m_shape = new btConeShape(m_radius, m_height);
	shapes->addChildShape(GetLocalTransform(), m_shape);
}

//=============================================================
// 円錐コライダー : 半径変更
//=============================================================
void ConeCollider::SetRadius(const float& radius)
{
	m_radius = radius;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}

//=============================================================
// 円錐コライダー : 高さ変更
//=============================================================
void ConeCollider::SetHeight(const float& height)
{
	m_height = height;
	gameObject->GetComponent<Collision>()->GetUpdateFlag() = true;
}
