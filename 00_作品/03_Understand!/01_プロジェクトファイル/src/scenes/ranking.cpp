//------------------------------------------------------------
// @file		ranking.cpp
// @brief	ランキングシーン
//------------------------------------------------------------
#include "ranking.h"

#include "components/2d/text.h"
#include "scripts/ranking_system.h"
#include "scenes/title.h"

//=============================================================
// 初期化
//=============================================================
void RankingScene::Init()
{
	GameObject* title = new GameObject("Title");
	title->AddComponent<TextUI>()->SetText("ランキング");
	title->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
	title->transform->position = { SCREEN_WIDTH * 0.5f, 10.0f, 0.0f };

	for (int i = 0; i < RankingSystem::MAX_DATA; i++)
	{
		float score = RankingSystem::GetInstance().GetScore(i);
		char scoreText[128];
		sprintf(&scoreText[0], "%d位: %.1f 点", i+1, score);

		if (score != -1.0f)
		{
			GameObject* rankingScore = new GameObject("RankingScore");
			rankingScore->AddComponent<TextUI>()->SetText(scoreText);
			rankingScore->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
			rankingScore->transform->position = { SCREEN_WIDTH * 0.5f, 200.0f + 100.0f * i, 0.0f };
		}
	}

	// BGM再生
	GameObject* bgm = new GameObject("BGM");
	bgm->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\BGM\\result_00.wav", true, true), Group::BGM);
	bgm->GetComponent<AudioSource>()->SetVolume(0.2f);
}

//=============================================================
// 更新
//=============================================================
void RankingScene::Update()
{
	if (AppInput->GetKeyDown(KeyCode::Return) || AppInput->GetKeyDown(KeyCode::PadB))
	{
		GameObject* se = new GameObject("SE", TAG_NODESTROY);
		se->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\SE\\interact.mp3", false, false), Group::SFX, true);
		AppScene->SetScene<TitleScene>();
	}
}
