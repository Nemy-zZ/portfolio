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
#include "heart.h"
#include "sound.h"

//初期化
int SceneModeGame::s_score = 0;

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeGame::Init()
{
	//スコアの初期化
	s_score = 0;
	
	//タイマーの初期化
	m_timer = timeGetTime();

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

	{
		//ハートテスト
		Object* heart = new Object("HeartUI");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(heart);
		pTransform2D->SetPos({ 60.0f, 60.0f });
		heart->AddComponent(pTransform2D);

		//ハートマネージャーを追加する
		HeartManager* pHeartManager = new HeartManager(heart);
		heart->AddComponent(pHeartManager);

		//初期化する
		heart->Init();
	}

	//スコアを生成する
	m_pScore = ScorePrefab::Create({ SCREEN_WIDTH - 400.0f, 60.0f }, {400.0f, 80.0f});

	//ステージを読み込む
	Manager::Get()->GetStage()->Load(Stage::TYPE::IS_MAIN);

	//フェードを生成する
	FadePrefab::Create();

	//BGM再生
	PlaySound(SOUND_LABEL_GAME);
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

	//BGM停止
	StopSound();
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
		Physics::Get()->Calc();
	}

	//ゲームオーバー処理
	Object* heartUI = Object::Find("HeartUI");
	if (!heartUI->GetComponent<HeartManager>()->IsAlive())
	{ //死んでいるとき
		//シーンの遷移
		this->GetScene()->Next(Scene::MODE::IS_RESULT);
	}

	//時間分のスコアを加算する
	if (timeGetTime() - m_timer >= 1000)
	{
		s_score += 100;
		m_timer = timeGetTime();
	}

	//スコアを更新する
	m_pScore->GetComponent<Number>()->SetNumber(s_score);
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeGame::Draw()
{
	//オブジェクトの一括描画を行う
	Object::DrawAll();
}