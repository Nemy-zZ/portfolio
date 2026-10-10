//==========================================================================================================
//
// ハートの処理[heart.h]
// Author ItouKotaro
//
//==========================================================================================================
#ifndef _HEART_H_
#define _HEART_H_

#include "behavior.h"

//ハートUI（単体）
class HeartUI : public Behavior	//（親）挙動
{
public:
	HeartUI(Object* pObject) : Behavior(pObject), m_active(true){}
	void Init() override;
	void Update() override{}
	void SetActive(const bool& active);
private:
	bool m_active;	// ハートが有効か
};

//ハートマネージャー
class HeartManager : public Behavior	//（親）挙動
{
public:
	HeartManager(Object* pObject) : Behavior(pObject), m_value(MAX_HEART), m_isAlive(true){}
	void Init() override;
	void Update() override;

	//ハート回復（+1）
	void RecoveryHeart();
	//ハートダメージ（-1）
	void DamageHeart();
	//体力全部のダメージを与える
	void AllDamageHeart();

	//生きているか
	bool IsAlive() { return m_isAlive; }

private:
	static const int MAX_HEART = 3;
	static const float HEART_SIZE;
	static const float HEART_SPACE;

	int m_value;									//ハート（体力）
	bool m_isAlive;								//生きているか
	Object* m_heartUI[MAX_HEART];	//ハートUI
};

#endif // !_HEART_H_
