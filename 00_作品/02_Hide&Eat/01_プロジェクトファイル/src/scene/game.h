//=============================================================
//
// ゲームシーン [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "scripts/present_generator.h"

// ゲームシーン
class CGameScene : public CScene
{
public:
	void Init() override;									// 初期化
	void Uninit() override;									// 終了
	void Update() override;								// 更新
	void Draw() override;									// 描画
	void onGameOver();
	bool IsGameOver() { return m_isGameOver; }
	void onClear();
	bool IsClear() { return m_isClear; }
private:
	GameObject* m_pCamera;							// カメラ
	GameObject* m_pPlayer;							// プレイヤー
	PresentGenerator* m_presentGenerator;		// プレゼントジェネレータ
	bool m_isGameOver;									// ゲームオーバー
	bool m_isClear;
	float m_gameTime;
};

#endif // !_GAME_H_
