//========================================================================================================
//
//ブロックスポナーの処理[block_spawner.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _BLOCK_SPAWNER_H_
#define _BLOCK_SPAWNER_H_

#include "behavior.h"

//ブロックスポナー
class BlockSpawner : public Behavior   //(親)挙動
{
public:
	static const int INTERVAL = 500;   //インターバル
	static const int NUM_SPAWN = 10;   //1回のスポーン数

	//コンストラクタ
	BlockSpawner(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nCntSpawn = 0;
	}

	//デストラクタ
	~BlockSpawner() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Spawn(float fYAxis);   //スポーン処理

private:
	int m_nCntSpawn;   //スポーンカウンター
};

#endif // !_BLOCK_SPAWNER_H_
