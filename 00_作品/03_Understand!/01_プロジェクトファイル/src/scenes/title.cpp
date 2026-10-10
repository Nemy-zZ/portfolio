//------------------------------------------------------------
// @file		title.cpp
// @brief	タイトルシーン
//------------------------------------------------------------
#include "title.h"

#include "components/2d/sprite.h"
#include "components/2d/text.h"
#include "scenes/game.h"

//=============================================================
// 初期化
//=============================================================
void TitleScene::Init()
{
	GameObject* background = new GameObject("BG");
	background->transform->size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	background->AddComponent<Sprite>()->SetTexture("data\\TEXTURE\\title_bg.png");

	GameObject* teamID = new GameObject("TeamID");
	teamID->AddComponent<TextUI>()->SetText("Team-04");
	teamID->GetComponent<TextUI>()->SetAlign(TextUI::RIGHT);
	teamID->transform->position = { SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT - 60.0f, 0.0f };

	GameObject* logo = new GameObject("Logo");
	logo->transform->size = { 1710.0f * 0.7f, 663.0f * 0.7f };
	logo->AddComponent<Sprite>()->SetTexture("data\\TEXTURE\\logo.png");
	logo->transform->position = { 380.0f, 315.0f, 0.0f };

	m_pressKey = new GameObject("PressKey");
	m_pressKey->AddComponent<TextUI>()->SetText("Press B Button!");
	m_pressKey->GetComponent<TextUI>()->SetAlign(TextUI::CENTER);
	m_pressKey->transform->position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 100.0f, 0.0f };

	// BGM再生
	GameObject* bgm = new GameObject("BGM");
	bgm->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\BGM\\title_00.wav", true, true), Group::BGM);
	bgm->GetComponent<AudioSource>()->SetVolume(0.2f);

	// チュートリアル表示
	m_tutorial = new GameObject("Tutorial");
	m_tutorial->transform->size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	m_tutorial->AddComponent<Sprite>();
	m_tutorial->SetActive(false);

	m_page = TUTORIAL_NONE;
}

//=============================================================
// 更新
//=============================================================
void TitleScene::Update()
{
	if (AppInput->GetKeyDown(KeyCode::Return) || AppInput->GetKeyDown(KeyCode::PadB))
	{
		m_page = (TUTORIAL)(m_page + 1);
		m_tutorial->SetActive(true);

		switch (m_page)
		{
		case TitleScene::TUTORIAL_1:
			m_tutorial->GetComponent<Sprite>()->SetTexture("data\\TEXTURE\\Tutorial_1.png");
			break;
		case TitleScene::TUTORIAL_2:
			m_tutorial->GetComponent<Sprite>()->SetTexture("data\\TEXTURE\\Tutorial_2.png");
			break;
		case TitleScene::TUTORIAL_3:
			m_tutorial->GetComponent<Sprite>()->SetTexture("data\\TEXTURE\\Tutorial_3.png");
			break;
		case TitleScene::TUTORIAL_ENDED:
			GameObject* se = new GameObject("SE", TAG_NODESTROY);
			se->AddComponent<AudioSource>()->Play(AudioClip::Ref2D("data\\SOUND\\SE\\interact.mp3", false, false), Group::SFX, true);
			AppScene->SetScene<GameScene>();
			break;
		}
	}
}
