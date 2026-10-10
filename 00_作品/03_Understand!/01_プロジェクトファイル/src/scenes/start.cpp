//------------------------------------------------------------
// @file		start.cpp
// @brief	最初のシーン
// @detail	起動直後に呼ばれるシーンの処理です
//------------------------------------------------------------
#include "start.h"
#include "system/manager.h"

// シーンのインクルード
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//=============================================================
// 初期化
//=============================================================
void StartScene::Init()
{
	// シーンマネージャーを取得する
	auto sceneManager = Manager::GetInstance()->GetSceneManager();

	// シーンの登録
	sceneManager->RegisterScene<TitleScene>();
	sceneManager->RegisterScene<GameScene>();
	sceneManager->RegisterScene<ResultScene>();
	sceneManager->RegisterScene<RankingScene>();

	// 遷移
	sceneManager->SetScene<TitleScene>();
}
