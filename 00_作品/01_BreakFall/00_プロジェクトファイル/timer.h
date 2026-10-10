//========================================================================================================
//
//タイマーの処理[timer.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "behavior.h"

//タイマークラス
class Timer : public Behavior   //(親)挙動
{
public:
	static const int START_TIMER;   //開始タイマー
	static const D3DXVECTOR2 SIZE;   //サイズ

	//コンストラクタ
	Timer(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nTimer = START_TIMER;   //タイマー
		this->m_dwTime = timeGetTime() / 1000;   //時間
	}

	//デストラクタ
	~Timer() override
	{

	}

	void Init() override;   //初期化
	void Update() override;   //更新処理

private:
	DWORD m_dwTime;   //時間
	int m_nTimer;   //タイマー
};

#endif // !_TIMER_H_