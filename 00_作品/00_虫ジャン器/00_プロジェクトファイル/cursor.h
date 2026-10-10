//========================================================================================================
//
//カーソルの処理[cursor.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _CORSOR_H_
#define _CORSOR_H_

#include "behavior.h"

//カーソル
class Cursor : public Behavior
{
public:
	static const float INIT_RANGE;   //初期範囲
	static const float ADD_RANGE;   //増加値
	static const float MOVE_SPEED;   //移動速度
	static const float MAX_DISTANCE;   //最大距離

	//コンストラクタ
	Cursor(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_pPlayer = nullptr;
		this->m_fRange = Cursor::INIT_RANGE;
	}

	//デストラクタ
	~Cursor() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

	float GetRange() { return this->m_fRange; };   //範囲の取得処理

private:
	void Control();   //操作処理

	Object* m_pPlayer;   //プレイヤー
	float m_fRange;   //範囲
};

#endif // !_CORSOR_H_