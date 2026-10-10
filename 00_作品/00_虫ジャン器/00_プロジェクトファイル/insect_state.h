//========================================================================================================
//
//虫の状態処理[insect_state.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _INSECT_STATE_H_
#define _INSECT_STATE_H_

#include "object.h"

//虫の状態
class InsectState
{
public:
	//コンストラクタ
	InsectState(Object* pInsect)
	{
		//メンバ変数を初期化する
		this->m_pInsect = pInsect;
	}

	//デストラクタ
	virtual ~InsectState()
	{
	}
	
	virtual void Handle() = 0;   //ハンドル
	virtual void Collision(Object* pObject) {};   //衝突時
	virtual void Call(Object* pPlayer) {};   //呼ばれる
	virtual void Throw(D3DXVECTOR3 posThrow, float fDireThrow, float fDistCursor) {};   //投げられる

protected:
	Object* GetIncect() { return this->m_pInsect; };   //虫の取得

private:
	Object* m_pInsect;   //虫
};

//虫のニュートラル状態
class InsectNeutralState : public InsectState
{
public:
	static const float DIS_SEARCH;   //サーチ距離

	//コンストラクタ
	InsectNeutralState(Object* pInsect) : InsectState(pInsect)
	{
	}

	//デストラクタ
	~InsectNeutralState() override
	{
	}

	void Handle() override;   //ハンドル
	void Call(Object* pPlayer) override;   //呼ぶ

private:
	void SeatchObject();   //オブジェクトを探す
	void SearchFruit();   //フルーツを探す
	void SearchObstacle();   //障害物を探す
};

//虫の追従状態
class InsectFollowState : public InsectState
{
public:
	static const float DIS_PLAYER;   //プレイヤーとの距離

	//コンストラクタ
	InsectFollowState(Object* pInsect, Object* pPlayer) : InsectState(pInsect)
	{
		//メンバ変数を初期化する
		this->pPlayer = pPlayer;
	}

	//デストラクタ
	~InsectFollowState() override
	{
	}

	void Handle() override;   //ハンドル
	void Throw(D3DXVECTOR3 posThrow, float fDireThrow, float fDistCursor) override;   //投げられる

private:
	Object* pPlayer;   //プレイヤー
};

//虫の投げられてる状態
class InsectThrowState : public InsectState
{
public:
	//コンストラクタ
	InsectThrowState(Object* pInsect) : InsectState(pInsect)
	{
	}

	//デストラクタ
	~InsectThrowState() override
	{
	}

	void Handle() override;   //ハンドル
	void Collision(Object* pObject) override;   //衝突時

private:
};

//虫の運搬状態
class InsectCarryState : public InsectState
{
public:
	//コンストラクタ
	InsectCarryState(Object* pInsect, Object* pCarry) : InsectState(pInsect)
	{
		//メンバ変数を初期化する
		this->m_pCarry = pCarry;   //運搬オブジェクト
	}

	//デストラクタ
	~InsectCarryState() override
	{
	}

	void Handle() override;   //ハンドル
	void Call(Object* pPlayer) override;   //呼ぶ

private:
	Object* m_pCarry;   //運搬オブジェクト
};

//虫の移動状態
class InsectMoveState : public InsectState
{
public:
	//コンストラクタ
	InsectMoveState(Object* pInsect, D3DXVECTOR3 posTarget) : InsectState(pInsect)
	{
		//メンバ変数を初期化する
		this->m_posTarget = posTarget;   //移動先
	}

	//デストラクタ
	~InsectMoveState() override
	{
	}

	void Handle() override;   //ハンドル
	void Call(Object* pPlayer) override;   //呼ぶ
	void Collision(Object* pObject) override;

private:
	D3DXVECTOR3 m_posTarget;   //移動先
};

//虫の攻撃状態
class InsectAttackState : public InsectState
{
public:
	static const int CNT_ATTACK;   //攻撃カウンター

	//コンストラクタ
	InsectAttackState(Object* pInsect, Object* pObjAttack) : InsectState(pInsect)
	{
		//メンバ変数を初期化する
		this->m_pObjAttack = pObjAttack;   //攻撃中のオブジェクト
		this->m_nCntAttack = CNT_ATTACK;   //攻撃カウンター
	}

	//デストラクタ
	~InsectAttackState() override
	{
	}

	void Handle() override;   //ハンドル
	void Call(Object* pPlayer) override;   //呼ぶ

private:
	Object* m_pObjAttack;   //攻撃中のオブジェクト
	int m_nCntAttack;   //攻撃カウンター
};

#endif // !_INSECT_STATE_H_