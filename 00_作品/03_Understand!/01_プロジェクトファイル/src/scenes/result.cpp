//------------------------------------------------------------
// @file		result.cpp
// @brief	リザルトシーン
//------------------------------------------------------------
#include "result.h"

#include "components/2d/text.h"
#include "scenes/game.h"
#include "scripts/ranking_system.h"
#include "scenes/ranking.h"

//=============================================================
// 初期化
//=============================================================
void ResultScene::Init()
{
	GameObject* title = new GameObject("Title");
	title->AddComponent<TextUI>()->SetText("Result");
	title->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
	title->GetComponent<TextUI>()->SetFontSize(150);
	title->transform->position = { SCREEN_WIDTH * 0.5f, 50.0f, 0.0f };

	// 経過時間
	m_elapsedTime = (timeGetTime() - AppScene->GetScene<GameScene>()->GetElapsedTime()) * 0.001f;

	char timeText[128];
	sprintf(&timeText[0], "%.1f 点", m_elapsedTime * AppScene->GetScene<GameScene>()->GetAveSpeed());

	GameObject* elapsedTime = new GameObject("ElapsedTime");
	elapsedTime->AddComponent<TextUI>()->SetText(timeText);
	elapsedTime->GetComponent<TextUI>()->SetFontSize(200);
	elapsedTime->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
	elapsedTime->transform->position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 30.0f, 0.0f };

	// ランキングに登録する
	RankingSystem::GetInstance().RegisterData(m_elapsedTime * AppScene->GetScene<GameScene>()->GetAveSpeed());
	int rank = RankingSystem::GetInstance().GetRanking(m_elapsedTime * AppScene->GetScene<GameScene>()->GetAveSpeed());

	if (rank != 0)
	{
		GameObject* rankText = new GameObject("RankText");
		rankText->AddComponent<TextUI>()->SetText(std::to_string(rank) + "位にランクインしました！");
		rankText->GetComponent<TextUI>()->SetFontSize(100);
		rankText->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
		rankText->transform->position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 150.0f, 0.0f };
	}

	// BGM再生
	GameObject* bgm = new GameObject("BGM");
	bgm->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\BGM\\result_00.wav", true, true), Group::BGM);
	bgm->GetComponent<AudioSource>()->SetVolume(0.2f);
}

//=============================================================
// 更新
//=============================================================
void ResultScene::Update()
{
	if (AppInput->GetKeyDown(KeyCode::Return) || AppInput->GetKeyDown(KeyCode::PadB))
	{
		GameObject* se = new GameObject("SE", TAG_NODESTROY);
		se->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\SE\\interact.mp3", false, false), Group::SFX, true);
		AppScene->SetScene<RankingScene>();
	}
}
