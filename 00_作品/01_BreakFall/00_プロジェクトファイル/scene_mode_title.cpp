//========================================================================================================
//
//タイトルシーン[scene_mode_title.h]
// Author MurataShinnosuke
//
//========================================================================================================
#include "scene_mode.h"
#include "prefab.h"
#include "menu.h"
#include "sound.h"

//定数
constexpr float END_LOGO_HEIGHT = 350.0f;	//ロゴの最終到達高さ

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeTitle::Init()
{
	//背景を生成する
	pBG = Background2DPrefab::Create(Manager::Get()->GetTexture()->Get("title_bg.png"));
	pBG->GetComponent<Transform2D>()->SetPos({ SCREEN_WIDTH / 2, 0.0f });
	pBG->GetComponent<Transform2D>()->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

	//タイトルメニューを生成する
	this->pTitleMenu = TitleMenuPrefab::Create();

	//フェードを生成する
	FadePrefab::Create();

	//BGM再生
	PlaySound(SOUND_LABEL_TITLE);
}

//========================================================================================================
//終了処理
//========================================================================================================
void SceneModeTitle::Uninit()
{
	//オブジェクトを一括破棄する
	Object::DestroyAll();

	//BGM停止
	StopSound();
}

//========================================================================================================
//更新処理
//========================================================================================================
void SceneModeTitle::Update()
{
	//オブジェクトを一括更新する
	Object::UpdateAll();

	//背景をおろす
	D3DXVECTOR2 bgPos = pBG->GetComponent<Transform2D>()->GetPos();
	if (bgPos.y < END_LOGO_HEIGHT)
	{
		bgPos.y += (END_LOGO_HEIGHT - bgPos.y) * 0.08f;
		pBG->GetComponent<Transform2D>()->SetPos(bgPos);
	}

	//決定フラグを確認する
	Menu* pMenu = this->pTitleMenu->GetComponent<Menu>();
	if (pMenu->GetDecide() == true)
	{//決定フラグが立っている場合
		//選択を確認する
		int nSelect = pMenu->GetSelect();
		if (nSelect == TitleMenu::MENU::IS_START)
		{//スタートの場合
			//画面遷移フラグを立てる
			this->GetScene()->Next(Scene::MODE::IS_TUTORIAL);
		}
		else if (nSelect == TitleMenu::MENU::IS_RANKING)
		{//ランキングの場合
			//画面遷移フラグを立てる
			this->GetScene()->Next(Scene::MODE::IS_RANKIGN);
		}

		//音を鳴らす
		PlaySound(SOUND_LABEL_SELECT_DECIDE);
	}

	//選択が変更されていたら効果音を鳴らす
	if (pMenu->GetSelect() != m_oldSelect)
	{
		//音を鳴らす
		PlaySound(SOUND_LABEL_SELECT_MOVE);

		//更新
		m_oldSelect = pMenu->GetSelect();
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeTitle::Draw()
{
	//オブジェクトを一括描画する
	Object::DrawAll();
}