//========================================================================================================
//
//ランキングシーン処理[scene_mode_ranking.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "scene_mode.h"
#include "scene.h"
#include "keyboard.h"
#include "joypad.h"
#include "prefab.h"
#include "ranking.h"
//#include "rank.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeRanking::Init()
{
	//背景を生成する
	{
		//背景を生成する
		Object* bg = new Object("Background2D");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(bg);
		pTransform2D->SetPos({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});
		pTransform2D->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT});
		bg->AddComponent(pTransform2D);

		//四角形レンダリングを追加する
		Square* pSquare = new Square(bg);
		pSquare->SetTexture(Manager::Get()->GetTexture()->Get("ranking_bg.png"));
		bg->AddComponent(pSquare);

		//初期化する
		bg->Init();
	}

	//ランキングを生成する
	for (int i = 0; i < RankingSystem::MAX_DATA; i++)
	{
		Object* rank = ScorePrefab::Create({ SCREEN_WIDTH /2 - 180.0f, 220.0f + i * 100.0f }, {400.0f, 80.0f});

		//スコア
		int score = RankingSystem::GetInstance()->GetScore(i);
		rank->GetComponent<Score>()->Add(score != -1 ? score : 0);
	}

	////順位を生成する
	//Rank::Create();

	//フェードを生成する
	FadePrefab::Create();
}

//========================================================================================================
//終了処理
//========================================================================================================
void SceneModeRanking::Uninit()
{
	//オブジェクトの一括破棄を行う
	Object::DestroyAll();
}

//========================================================================================================
//更新処理
//========================================================================================================
void SceneModeRanking::Update()
{
	//オブジェクトの一括更新を行う
	Object::UpdateAll();

	//操作処理を行う
	this->Control();
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeRanking::Draw()
{
	//オブジェクトの一括描画を行う
	Object::DrawAll();
}

//========================================================================================================
//操作
//========================================================================================================
void SceneModeRanking::Control()
{
	//----------------------------------------------------------------------------------------------------
	//入力処理を行う
	//----------------------------------------------------------------------------------------------------
	Keyboard* pKeyboard = Manager::Get()->GetKeyboard();   //キーボード
	Joypad* pJoypad = Manager::Get()->GetJoypad();   //ジョイパッド

	//SPASEキーを確認する
	if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTrigger(Joypad::JOYKEY::IS_A, 0))
	{//SPASEキーが押された場合
		//終了フラグを立てる
		this->GetScene()->Next(Scene::MODE::IS_TITLE);
	}
}