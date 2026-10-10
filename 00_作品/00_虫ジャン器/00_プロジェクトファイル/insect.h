//========================================================================================================
//
//虫の処理[insect.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _INSECT_H_
#define _INSECT_H_

#include "behavior.h"
#include "insect_state.h"

//虫
class Insect : public Behavior
{
public:
	static const float MOVE_SPEED;   //移動量
	static const float MOVE_THROW;   //投げられた時の移動量

	//コンストラクタ
	Insect(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_pState = new InsectNeutralState(pObject);
	}

	//デストラクタ
	~Insect() override
	{
		//状態を破棄する
		delete this->m_pState;
		this->m_pState = nullptr;
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void ChangeState(InsectState* state);   //状態の変更
	void Collision(Object* pObject, Physics::CONTACT contact) override { this->m_pState->Collision(pObject); };   //衝突時
	void Call(Object* pPlayer) { this->m_pState->Call(pPlayer); };   //呼ばれる
	void Throw(D3DXVECTOR3 posThrow, float fDireThrow, float fDistCursor) { this->m_pState->Throw(posThrow, fDireThrow, fDistCursor); };   //投げられる

private:
	InsectState* m_pState;   //状態
};

#endif // !_INSECT_H_
