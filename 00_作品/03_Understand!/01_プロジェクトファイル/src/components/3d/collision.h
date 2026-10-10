//------------------------------------------------------------
// @file			collision.h
// @brief		コリジョン
// @detail	当たり判定
//------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <component.h>
#include "collider.h"

/**
 * @brief コリジョンコンポーネント
 * @details 当たり判定の標準コンポーネントです（形状コライダーを追加すると、自動で追加されます）
*/
class Collision : public Component
{
public:
	Collision() : m_updateFlag(true), m_collision(nullptr), m_shape(nullptr), m_trigger(false), m_friction(1.0f) {}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void UpdateOldTransform();

	//@brief コリジョンオブジェクトを取得する
	btCollisionObject* GetCollision() { return m_collision; }

	//@brief コライダーを取得する
	btCompoundShape* GetShape() { return m_shape; }

	//@brief ビルドする
	virtual void Build();

	//@brief トリガーを設定する
	void SetTrigger(const bool& enabled);
	//@brief トリガーを取得する
	bool GetTrigger() { return m_trigger; }
	//@brief 摩擦を設定する
	void SetFriction(const float& friction);
	//@brief 摩擦を取得する
	float GetFriction() { return m_friction; }
	//@brief 更新フラグを取得する
	bool& GetUpdateFlag() { return m_updateFlag; }
	//@brief 前回のトランスフォーム情報を取得する
	Transform GetOldTransform() { return m_oldTransform; }
	//@brief 重なっているコリジョンを取得する
	std::vector<Collision*>& GetOverlappingCollisions() { return m_overlapping; }

protected:
	bool m_updateFlag;					// 更新フラグ
	btCollisionObject* m_collision;		// コリジョン
	btCompoundShape* m_shape;	// 形状

private:
	//@brief 合わせたコライダーを生成する
	void GenerateColliderShape();

	float m_friction;											// 摩擦
	bool m_trigger;											// トリガー
	Transform m_oldTransform;							// 前回のトランスフォーム情報
	std::vector<Collision*> m_overlapping;		// 重なっているコリジョン
};

/**
 * @brief リジッドボディコンポーネント（コライダーを先にアタッチしてください）
 * @details 物理挙動するようになります
*/
class RigidBody : public Component
{
public:
	RigidBody() : m_mass(1.0f), m_motionState(nullptr), m_isAlwayActive(false){}
	void Init() override;
	void Uninit() override;
	void Update() override;

	//@brief リジッドボディを取得する
	btRigidBody* GetRigidBody();

	//@brief 質量を設定する
	void SetMass(const float& mass);
	//@brief 質量を取得する
	float GetMass() { return m_mass; }
	//@brief 常にアクティブを 有効/無効 にする
	void SetAlwayActive(const bool& enabled);
	//@brief 常にアクティブを取得する
	bool GetAlwayActive() { return m_isAlwayActive; }

	//@brief モーションステートを取得する
	void SetMotionState(btMotionState* motionState);

	//@brief モーションステートを取得する
	btMotionState* GetMotionState() { return m_motionState; }

private:
	float m_mass;							// 質量
	bool m_isAlwayActive;				// 常にアクティブ
	btMotionState* m_motionState;	// モーションステート
};

//// ゴーストオブジェクト
//class CGhostObject : public Component
//{
//public:
//	void Init() override;							// 初期化
//	btGhostObject* GetGhostObject();		// ゴーストオブジェクトの取得
//	Collision* GetCollision();					// コリジョンの取得
//};
//
//// 点球結合
//class CPoint2PointConstraint : public Component
//{
//public:
//	CPoint2PointConstraint();
//	void Uninit() override;
//	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB);
//private:
//	btPoint2PointConstraint* m_p2p;
//};
//
//// ヒンジ結合
//class CHingeConstraint : public Component
//{
//public:
//	CHingeConstraint();
//	void Uninit() override;
//	void SetConstraint(btRigidBody* rb, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& axisInA);
//	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB, const D3DXVECTOR3& axisInA, const D3DXVECTOR3& axisInB);
//	btHingeConstraint* GetHinge() { return m_hinge; }
//private:
//	btHingeConstraint* m_hinge;
//};
//
//// 2軸ヒンジ結合
//class CHinge2Constraint : public Component
//{
//public:
//	CHinge2Constraint();
//	void Uninit() override;
//	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, D3DXVECTOR3 anchor, D3DXVECTOR3 parentAxis, D3DXVECTOR3 childAxis);
//	btHinge2Constraint* GetHinge2() { return m_hinge2; }
//private:
//	btHinge2Constraint* m_hinge2;
//};
//
//// コライダー基底クラス
//class CCollider : public Component
//{
//public:
//	virtual void AddShapes(btCompoundShape* pShapes) = 0;						// 形状追加
//	void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }			// オフセット設定
//	D3DXVECTOR3 GetOffset() { return m_offset; }										// オフセット取得
//	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }							// 回転設定
//	D3DXVECTOR3 GetRot() { return m_rot; }												// 回転取得
//private:
//	D3DXVECTOR3 m_offset;		// 中心
//	D3DXVECTOR3 m_rot;			// 回転
//};
//
//
//// ボックスコライダー
//class CBoxCollider : public CCollider
//{
//public:
//	CBoxCollider(D3DXVECTOR3 size = { 1.0f, 1.0f, 1.0f }, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });		// コンストラクタ
//	void Init() override;																						// 初期化
//	void AddShapes(btCompoundShape* pShapes) override;									// 形状追加
//
//	void SetSize(const D3DXVECTOR3& size) { m_size = size; }								// サイズ設定
//	D3DXVECTOR3 GetSize() { return m_size; }														// サイズ取得
//private:
//	D3DXVECTOR3 m_size;		// サイズ
//};
//
//// 球コライダー
//class CSphereCollider : public CCollider
//{
//public:
//	CSphereCollider(float fRadius = 1.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
//	void Init() override;																					// 初期化
//	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加
//
//	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
//	float GetRadius() { return m_fRadius; }														// 半径取得
//private:
//	float m_fRadius;		// 半径
//};
//
//// カプセルコライダー
//class CCapsuleCollider : public CCollider
//{
//public:
//	CCapsuleCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
//	void Init() override;																					// 初期化
//	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加
//
//	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
//	float GetRadius() { return m_fRadius; }														// 半径取得
//	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
//	float GetHeight() { return m_fHeight; }														// 高さ取得
//private:
//	float m_fRadius;		// 半径
//	float m_fHeight;		// 高さ
//};
//
//// シリンダー(円柱) コライダー
//class CCylinderCollider : public CCollider
//{
//public:
//	CCylinderCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
//	void Init() override;																					// 初期化
//	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加
//
//	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
//	float GetRadius() { return m_fRadius; }														// 半径取得
//	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
//	float GetHeight() { return m_fHeight; }														// 高さ取得
//private:
//	float m_fRadius;		// 半径
//	float m_fHeight;		// 高さ
//};
//
//// コーン(円錐) コライダー
//class CConeCollider : public CCollider
//{
//public:
//	CConeCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
//	void Init() override;																					// 初期化
//	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加
//
//	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
//	float GetRadius() { return m_fRadius; }														// 半径取得
//	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
//	float GetHeight() { return m_fHeight; }														// 高さ取得
//private:
//	float m_fRadius;		// 半径
//	float m_fHeight;		// 高さ
//};

#endif // !_COLLISION_H_
