//========================================================================================================
//
//障害物の処理[obstacle.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "behavior.h"

//障害物
class Obstacle : public Behavior   //(親)挙動
{
public:
	static const D3DXVECTOR3 SUB_SCALE;   //拡大率の減少量

	//コンストラクタ
	Obstacle(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nHp = 5;   //ヒットポイント
		this->m_isBreak = false;   //破壊フラグ
	}

	//デストラクタ
	~Obstacle() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Hit(int nDamage);   //ヒット

	bool GetBreak() { return this->m_isBreak; };   //破壊フラグの取得

private:
	int m_nHp;   //ヒットポイント
	bool m_isBreak;   //破壊フラグ
};

#endif // !_OBSTACLE_H_
