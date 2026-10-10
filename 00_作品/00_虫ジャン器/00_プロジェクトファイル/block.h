//========================================================================================================
//
//ブロックの処理[block.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "behavior.h"

//ブロッククラス
class Block : public Behavior   //(親)挙動
{
public:
	static const D3DXVECTOR3 MODEL_SIZE;   //モデルのサイズ

	//コンストラクタ
	Block(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Block() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:
};

#endif // !_BLOCK_H_
