//========================================================================================================
//
//ゲームシーン[scene_mode_game.h]
// Author MurataShinnosuke
//
//========================================================================================================
#include "scene_mode.h"
#include "prefab.h"
#include "physics.h"
#include "waypoint.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeGame::Init()
{
	//当たり判定を生成する
	Physics::Create();

	//ポーズを生成する
	this->m_pPause = new Pause;
	this->m_pPause->Init();   //初期化

	//カメラを生成する
	CameraPrefab::Create();

	//ディレクショナルライトを生成する
	DireLightPrefab::Create({ 0.22f, -0.87f, 0.44f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	DireLightPrefab::Create({ -0.18f, 0.88f, -0.44f }, { 0.65f, 0.65f, 0.65f, 1.0f });
	DireLightPrefab::Create({ 0.89f, 0.11f, 0.44f }, { 0.15f, 0.15f, 0.15f, 0.15f });

	//ステージを読み込む
	Manager::Get()->GetStage()->Load(Stage::TYPE::IS_DEMO);

	//フェードを生成する
	FadePrefab::Create();
}

//========================================================================================================
//終了処理
//========================================================================================================
void SceneModeGame::Uninit()
{
	//ウェイポイントを一括解放する
	Waypoint::ReleaseAll();

	//オブジェクトを一括破棄する
	Object::DestroyAll();

	//ポーズを破棄する
	this->m_pPause->Uninit();
	delete this->m_pPause;
	this->m_pPause = nullptr;

	//当たり判定を破棄する
	Physics::Destroy();
}

//========================================================================================================
//更新処理
//========================================================================================================
void SceneModeGame::Update()
{
	Keyboard* pKeyboard = Manager::Get()->GetKeyboard();   //キーボード入力
	Joypad* pJoypad = Manager::Get()->GetJoypad();   //ジョイパッド

	//Pキーを確認する
	if (pKeyboard->GetTrigger(DIK_P) || pJoypad->GetTrigger(Joypad::IS_START, 0))
	{//Pキーが押されている場合
		//ポーズフラグを切り替える
		this->m_pPause->SwitchPause();
	}

	//ポーズフラグを確認する
	if (this->m_pPause->GetPause() == true)
	{//ポーズフラグが立っている場合
		//ポーズの更新処理を行う
		this->m_pPause->Update();

		//ポーズメニューを取得する
		Menu* pPauseMenu = this->m_pPause->GetMenu();

		//終了フラグを確認する
		if (pPauseMenu->GetDecide() == true)
		{//終了フラグが立っている場合
			//選択を確認する
			int nSelect = pPauseMenu->GetSelect();
			if (nSelect == PauseMenu::MENU::IS_CONTINUE)
			{//コンテニューの場合
				//ポーズを切り替える
				this->m_pPause->SwitchPause();
			}
			else if (nSelect == PauseMenu::MENU::IS_REPLAY)
			{//リプレイの場合
				//ゲーム画面に遷移する
				this->GetScene()->Next(Scene::MODE::IS_GAME);
			}
			else if (nSelect == PauseMenu::MENU::IS_QUIT)
			{//終了の場合
				//タイトル画面に遷移する
				this->GetScene()->Next(Scene::MODE::IS_TITLE);
			}
		}
	}
	else
	{//立っていない場合
		Object::UpdateAll();
		Physics::Get()->Calc();   //当たり判定
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeGame::Draw()
{
	//オブジェクトの一括描画を行う
	Object::DrawAll();
}