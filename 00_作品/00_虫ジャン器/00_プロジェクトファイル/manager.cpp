//========================================================================================================
//
//マネージャー処理[manager.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "manager.h"
#include "object.h"

#include <time.h>

//静的メンバ変数を初期化する
Manager* Manager::m_pInstance = nullptr;

//=======================================================================================================
//初期化処理
//=======================================================================================================
HRESULT Manager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーを生成する
	this->m_pRenderer = new Renderer;
	this->m_pRenderer->Init(hWnd, TRUE);   //初期化

	//テクスチャを生成する
	this->m_pTexture = new Texture;
	this->m_pTexture->Init();   //初期化

	//Xファイルを生成する
	this->m_pXFile = new XFile;
	this->m_pXFile->Init();   //初期化

	//キーボードを生成する
	this->m_pKeyboard = new Keyboard;
	this->m_pKeyboard->Init(hInstance, hWnd);   //初期化

	//マウスを生成する
	this->m_pMouse = new Mouse;
	this->m_pMouse->Init(hInstance, hWnd);   //初期化

	//ジョイパッドを生成する
	this->m_pJoypad = new Joypad;
	this->m_pJoypad->Init(hInstance, hWnd);   //初期化

	//ステージを生成する
	this->m_pStage = new Stage;

	//シーンを生成する
	this->m_pScene = new Scene;
	this->m_pScene->ChangeMode(new SceneModeTitle(this->m_pScene));   //タイトルシーンに変更

	return S_OK;
}

//=======================================================================================================
//終了処理
//=======================================================================================================
void Manager::Uninit()
{
	//ステージを破棄する
	delete this->m_pStage;
	this->m_pStage = nullptr;

	//シーンを破棄する
	this->m_pScene->Uninit();   //終了処理
	delete this->m_pScene;
	this->m_pScene = nullptr;

	//ジョイパッドを破棄する
	this->m_pJoypad->Uninit();
	delete this->m_pJoypad;
	this->m_pJoypad = nullptr;

	//マウスを破棄する
	this->m_pMouse->Uninit();
	delete this->m_pMouse;
	this->m_pMouse = nullptr;

	//キーボードを破棄する
	this->m_pKeyboard->Uninit();
	delete this->m_pKeyboard;
	this->m_pKeyboard = nullptr;

	//Xファイルを破棄する
	this->m_pXFile->Uninit();   //終了処理
	delete this->m_pXFile;
	this->m_pXFile = nullptr;

	//テクスチャを破棄する
	this->m_pTexture->Uninit();   //終了処理
	delete this->m_pTexture;
	this->m_pTexture = nullptr;

	//レンダラーを破棄する
	this->m_pRenderer->Uninit();
	delete this->m_pRenderer;
	this->m_pRenderer = nullptr;
}

//=======================================================================================================
//更新処理
//=======================================================================================================
void Manager::Update()
{
	//乱数の種を設定
	srand((unsigned int)clock());

	//キーボードの更新処理を行う
	this->m_pKeyboard->Update();

	//マウスの更新処理を行う
	this->m_pMouse->Update();

	//ジョイパッドの更新処理を行う
	this->m_pJoypad->Update();

	//レンダラーの更新処理を行う
	this->m_pRenderer->Update();

	//シーンの更新処理を行う
	this->m_pScene->Update();
}

//=======================================================================================================
//描画処理
//=======================================================================================================
void Manager::Draw()
{
	//レンダラーの描画処理を行う
	this->m_pRenderer->Draw();

	//シーンの遷移フラグを確認する
	if (this->m_pScene->GetNext())
	{//遷移フラグが立っている場合
		//次のシーンを確認する
		Scene::MODE modeNext = this->m_pScene->GetNextMode();
		if (modeNext == Scene::MODE::IS_TITLE)
		{//タイトルの場合
			//タイトル画面に変更する
			this->m_pScene->ChangeMode(new SceneModeTitle(this->m_pScene));
		}
		else if (modeNext == Scene::MODE::IS_GAME)
		{//ゲーム画面の場合
			//ゲーム画面に変更する
			this->m_pScene->ChangeMode(new SceneModeGame(this->m_pScene));
		}
		else if (modeNext == Scene::MODE::IS_RESULT)
		{//リザルト画面の場合
			//リザルト画面に変更する
			this->m_pScene->ChangeMode(new SceneModeResult(this->m_pScene));
		}
		else if (modeNext == Scene::MODE::IS_RANKIGN)
		{//ランキング画面の場合
			//ランキング画面に変更する
			this->m_pScene->ChangeMode(new SceneModeResult(this->m_pScene));
		}
	}
}