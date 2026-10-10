//=============================================================
//
// ランキング [ranking_system.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RANKING_SYSTEM_H_
#define _RANKING_SYSTEM_H_

// ランキングシステム
class RankingSystem
{
public:
	// インスタンスを取得する
	static RankingSystem& GetInstance()
	{
		static RankingSystem instance;
		return instance;
	}

	// データ登録
	void RegisterData(float score);

	// 順位を取得する
	int GetRanking(float score);

	// 順位からスコアを取得する
	float GetScore(int rank);

	static const std::string RANKING_PATH;	// ランキングのパス
	static const int MAX_DATA;						// 最大データ
private:
	RankingSystem() = default;
};

#endif // !_RANKING_SYSTEM_H_
