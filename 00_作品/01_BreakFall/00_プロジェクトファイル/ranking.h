//==========================================================================================================
//
// ランキングの処理[ranking.h]
// Author ItouKotaro
//
//==========================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include <iostream>

// ランキングシステム
class RankingSystem
{
public:
	// インスタンスの取得
	static RankingSystem* GetInstance()
	{
		static RankingSystem instance;
		return &instance;
	}

	// データ登録
	void RegisterData(int score);

	// 順位を取得する
	int GetRanking(int score);

	// 順位からスコアを取得する
	int GetScore(int rank);

	static const std::string RANKING_PATH;	// ランキングのパス
	static const int MAX_DATA;						// 最大データ
private:

};

#endif // !_RANKING_H_
