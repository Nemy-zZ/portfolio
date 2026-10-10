//========================================================================================================
//
//スコア[score.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "behavior.h"

//スコアクラス
class Score : public Behavior   //(親)挙動
{
public:
	//スコア
	Score(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nScore = 0;   //スコア
	}

	//デストラクタ
	~Score() override
	{
	}

	void Init() override;   //初期化
	void Update() override;   //更新処理
	void Add(int nValue);   //加算処理

private:
	int m_nScore;   //スコア
};

#endif // !_SCORE_H_