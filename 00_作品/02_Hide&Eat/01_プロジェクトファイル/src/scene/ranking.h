//=============================================================
//
// ゲームシーン [ranking.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "scene.h"

// ランキングシーン
class CRankingScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
	void SetRank(const int& rank) { m_rank = rank; }
private:
	int m_rank;
};

#endif // !_RANKING_H_
