//------------------------------------------------------------
// @file			collider.h
// @brief		コライダー
// @detail	当たり判定の形状
//------------------------------------------------------------
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "component.h"

/**
 * @brief コライダーテンプレート
*/
class Collider : public Component
{
public:
	Collider();
	void Init() override;
	void Uninit() override;

	//@brief シェイプに追加する
	virtual void AddShape(btCompoundShape* shapes) = 0;

	//@brief オフセットを設定する
	void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }
	//@brief 回転を設定する
	void SetRotation(const D3DXQUATERNION& rotation) { m_rotation = rotation; }
protected:
	//@brief ローカル姿勢情報をトランスフォームとして取得します
	btTransform GetLocalTransform();

	D3DXVECTOR3 m_offset;			// 中心
	D3DXQUATERNION m_rotation;	// 回転
	btCollisionShape* m_shape;		// 形状
};

/**
 * @brief ボックスコライダー
 * @details 長方形のコライダーです
*/
class BoxCollider : public Collider
{
public:
	BoxCollider() : m_size(D3DXVECTOR3(1.0f, 1.0f, 1.0f)){}
	void AddShape(btCompoundShape* shapes) override;
	void SetSize(const D3DXVECTOR3& size);
private:
	D3DXVECTOR3 m_size;	// サイズ
};

/**
 * @brief スフィアコライダー
 * @details 球のコライダーです
*/
class SphereCollider : public Collider
{
public:
	SphereCollider() : m_radius(1.0f){}
	void AddShape(btCompoundShape* shapes) override;
	void SetRadius(const float& radius);
private:
	float m_radius;	// 半径
};

/**
 * @brief カプセルコライダー
 * @details カプセルのコライダーです
*/
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider() : m_radius(1.0f), m_height(2.0f) {}
	void AddShape(btCompoundShape* shapes) override;
	void SetRadius(const float& radius);
	void SetHeight(const float& height);
private:
	float m_radius;	// 半径
	float m_height;	// 高さ
};

/**
 * @brief 円柱コライダー
 * @details 円柱のコライダーです
*/
class CylinderCollider : public Collider
{
public:
	CylinderCollider() : m_radius(1.0f), m_height(2.0f) {}
	void AddShape(btCompoundShape* shapes) override;
	void SetRadius(const float& radius);
	void SetHeight(const float& height);
private:
	float m_radius;	// 半径
	float m_height;	// 高さ
};

/**
 * @brief 円錐コライダー
 * @details 円錐のコライダーです
*/
class ConeCollider : public Collider
{
public:
	ConeCollider() : m_radius(1.0f), m_height(2.0f) {}
	void AddShape(btCompoundShape* shapes) override;
	void SetRadius(const float& radius);
	void SetHeight(const float& height);
private:
	float m_radius;	// 半径
	float m_height;	// 高さ
};


#endif // !_COLLIDER_H_
