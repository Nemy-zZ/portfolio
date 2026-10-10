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
	static const float MOVE;   //移動量
	static const D3DXVECTOR3 SIZE;   //サイズ

	//色の列挙
	enum COLOR
	{
		IS_RED = 0,   //赤
		IS_BLUE,   //青
		IS_GREEN,   //緑
		IS_YELLOW   //黄
	};

	//コンストラクタ
	Block(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nHp = 1;
	}

	//デストラクタ
	~Block() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void Hit();   //ヒット処理

private:
	int m_nHp;   //ヒットポイント
};

#endif // !_BLOCK_H_
