//========================================================================================================
//
//衝突範囲の処理[collider.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "component.h"
#include "physics.h"
#include "line.h"
#include "object.h"
#include "transform.h"

//衝突範囲クラス
class Collider : public Component
{
public:
	static const int CALC_PHYSICS = 5;   //当たり判定の演算回数

	typedef enum
	{
		IS_BOX = 0,   //ボックス
		IS_SPHERE,   //球体
	}TYPE;   //種類

	//コンストラクタ
	Collider(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_type = Collider::TYPE::IS_BOX;
		this->m_isTrigger = false;
		this->m_offset = { 0.0f, 0.0f, 0.0f };
		this->m_nMass = 3;   //質量
		this->m_isCollision = false;
	}

	//デストラクタ
	virtual ~Collider()
	{
	}

	virtual void Init() override = 0;   //初期化処理
	virtual void Uninit() override = 0;   //終了処理
	virtual void Update() override = 0;   //更新処理
	virtual void Draw() override = 0;   //描画処理
	virtual void Collision(Collider* pColliderTarget, Physics::CONTACT contact) = 0;   //衝突時の処理
	void Trigger() { this->m_isTrigger = true; };

	void SetCenter(D3DXVECTOR3 center) { this->GetObj()->GetComponent<Transform>()->SetPos(center - this->m_offset); };   //中央の設定処理
	void SetMass(int nMass) { this->m_nMass = nMass; };   //質量の設定処理
	void SetOffset(D3DXVECTOR3 offset) { this->m_offset = offset; };
	void SetCollision(bool isCollision) { this->m_isCollision = isCollision; };

	Collider::TYPE GetType() { return this->m_type; };   //種類の取得処理
	bool GetTrigger() { return this->m_isTrigger; };   //トリガーフラグの取得処理
	int GetMass() { return this->m_nMass; };
	D3DXVECTOR3 GetCenter() { return this->GetObj()->GetComponent<Transform>()->GetPos() + this->m_offset; };   //中央の取得処理
	D3DXVECTOR3 GetCenterOld() { return this->GetObj()->GetComponent<Transform>()->GetPosOld() + this->m_offset; };
	bool GetCollision() { return this->m_isCollision; };

protected:
	void SetType(Collider::TYPE type) { this->m_type = type; };

private:
	Collider::TYPE m_type;
	bool m_isTrigger;   //トリガーフラグ
	D3DXVECTOR3 m_offset;   //相対値
	int m_nMass;   //質量
	bool m_isCollision;
};

//ボックス衝突範囲
class BoxCollider : public Collider   //(親)衝突範囲クラス
{
public:
	static const int NUM_LINE = 12;   //辺の数

	typedef struct
	{
		D3DXVECTOR3 posMax;   //最大範囲
		D3DXVECTOR3 posMaxOld;   //前回の最大範囲
		D3DXVECTOR3 posMin;   //最小範囲
		D3DXVECTOR3 posMinOld;   //前回の最小範囲
	}Range;   //範囲

	//コンストラクタ
	BoxCollider(Object* pObject) : Collider(pObject)
	{
		//メンバ変数を初期化する
		this->m_size = {0.0f, 0.0f, 0.0f};   //サイズ
		this->m_range.posMax = { 0.0f, 0.0f, 0.0f };   //最大範囲
		this->m_range.posMaxOld = m_range.posMax;   //前回の最大範囲
		this->m_range.posMin = { 0.0f, 0.0f, 0.0f };   //最小範囲
		this->m_range.posMinOld = m_range.posMin;   //前回の最小範囲
		this->SetType(Collider::TYPE::IS_BOX);

		for (int i = 0; i < BoxCollider::NUM_LINE; i++)
		{
			//線を初期化する
			this->m_pLine[i] = nullptr;
		}
	}

	//デストラクタ
	~BoxCollider() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理
	void Collision(Collider* pColliderTarget, Physics::CONTACT contact) override;   //衝突時の処理

	void SetSize(D3DXVECTOR3 size) { this->m_size = size; };   //サイズ

	D3DXVECTOR3 GetSize() { return m_size; };   //サイズの取得処理
	Range GetRange() { return m_range; };   //範囲の取得処理

private:
	void UpdateRange();
	void UpdateLine();   //線の位置の設定処理

	D3DXVECTOR3 m_size;   //サイズ
	Range m_range;   //範囲
	Line* m_pLine[BoxCollider::NUM_LINE];   //線
};

//球体衝突範囲
class SphereCollider : public Collider   //(親)衝突範囲
{
public:
	//コンストラクタ
	SphereCollider(Object* pObject, float fRadius) : Collider(pObject)
	{
		//メンバ変数を初期化する
		this->SetType(Collider::TYPE::IS_SPHERE);   //種類
		this->m_fRadius = fRadius;   //半径
	}

	//デストラクタ
	~SphereCollider() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理
	void Collision(Collider* pColliderTarget, Physics::CONTACT contact) override;   //衝突時の処理


	float GetRadius() { return this->m_fRadius; };   //半径の取得

private:
	float m_fRadius;   //半径
};

#endif // !_COLLIDER_H_
