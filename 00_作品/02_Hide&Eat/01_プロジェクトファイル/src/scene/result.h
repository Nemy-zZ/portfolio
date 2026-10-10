//=============================================================
//
// リザルトシーン [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

// リザルトシーン
class CResultScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
	void SetScore(const int& score) { m_score = score; }
private:
	GameObject* m_timeNumText;
	int m_score;
};

#endif // !_RESULT_H_
