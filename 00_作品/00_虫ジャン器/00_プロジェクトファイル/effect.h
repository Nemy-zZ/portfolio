//========================================================================================================
//
//エフェクト処理[effect.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "component.h"

//エフェクト
class Effect : public Component   //(親)コンポーネント
{
public:
	//コンストラクタ
	Effect(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_fRadius = 0.0f;   //半径
	}

	//デストラクタ
	~Effect() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:
	float m_fRadius;   //半径
};

#endif // !_EFFECT_H_
