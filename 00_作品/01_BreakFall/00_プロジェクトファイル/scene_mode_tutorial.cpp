//========================================================================================================
//
//チュートリアルシーン[scene_mode_tutorial.h]
// Author ItoKotaro
//
//========================================================================================================
#include "scene_mode.h"
#include "prefab.h"
#include "sound.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void SceneModeTutorial::Init()
{
	//背景を生成する
	m_pTutorial = Background2DPrefab::Create(Manager::Get()->GetTexture()->Get("tutorial0.png"));
	m_pTutorial->GetComponent<Transform2D>()->SetPos({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	m_pTutorial->GetComponent<Transform2D>()->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

	//フェードを生成する
	FadePrefab::Create();
}

//========================================================================================================
//終了処理
//========================================================================================================
void SceneModeTutorial::Uninit()
{
	//オブジェクトを一括破棄する
	Object::DestroyAll();
}

//========================================================================================================
//更新処理
//========================================================================================================
void SceneModeTutorial::Update()
{
	//オブジェクトを一括更新する
	Object::UpdateAll();

	Keyboard* pKeyboard = Manager::Get()->GetKeyboard();
	Joypad* pJoypad = Manager::Get()->GetJoypad();
	
	//次のページへ
	if (pKeyboard->GetTrigger(DIK_SPACE) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		m_page++;

		//ページのテクスチャ変更
		switch (m_page)
		{
		case 1:
			m_pTutorial->GetComponent<Square>()->SetTexture(Manager::Get()->GetTexture()->Get("tutorial1.png"));
			break;
		case 2:
			//画面遷移フラグを立てる
			this->GetScene()->Next(Scene::MODE::IS_GAME);
			break;
		}

		//音を鳴らす
		PlaySound(SOUND_LABEL_SELECT_DECIDE);
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void SceneModeTutorial::Draw()
{
	//オブジェクトを一括描画する
	Object::DrawAll();
}