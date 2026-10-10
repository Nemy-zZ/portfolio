//========================================================================================================
//
//リザルトシーン[scene_mode_game.h]
// Author MurataShinnosuke
//
//========================================================================================================
#include "scene_mode.h"
#include "scene.h"
#include "prefab.h"
#include "ranking.h"
#include "sound.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeResult::Init()
{
	//背景を生成する
	Background2DPrefab::Create(Manager::Get()->GetTexture()->Get("result_bg.png"));

	//スコアをランキングに登録する
	RankingSystem::GetInstance()->RegisterData(SceneModeGame::GetScore());

	//スコアを表示する
	Object* score = ScorePrefab::Create({ SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2 }, {800.0f, 150.0f});
	score->GetComponent<Score>()->Add(SceneModeGame::GetScore());

	//フェードを生成する
	FadePrefab::Create();

	//BGM再生
	PlaySound(SOUND_LABEL_RESULT);
}

//========================================================================================================
//終了処理
//========================================================================================================
void SceneModeResult::Uninit()
{
	//オブジェクトを一括破棄する
	Object::DestroyAll();

	//BGM停止
	StopSound();
}

//========================================================================================================
//更新処理
//========================================================================================================
void SceneModeResult::Update()
{
	Keyboard* pKeyboard = Manager::Get()->GetKeyboard();   //キーボード

	//オブジェクトを一括更新する
	Object::UpdateAll();

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{//Enterキーが押された場合
		//終了フラグを立てる
		this->GetScene()->Next(Scene::MODE::IS_TITLE);
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeResult::Draw()
{
	//オブジェクトを一括描画する
	Object::DrawAll();
}