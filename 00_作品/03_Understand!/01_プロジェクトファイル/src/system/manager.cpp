//------------------------------------------------------------
// @file		manager.cpp
// @brief	マネージャー
// @detail	システムの管理を行います
//------------------------------------------------------------
#include "manager.h"
#include "components/2d/sprite.h"
#include "components/2d/text.h"

//=============================================================
// 初期化
//=============================================================
void Manager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// レンダラーを生成する
	m_renderer = new Renderer();
	m_renderer->Init(hInstance, hWnd);

	// 物理を生成する
	m_physics = new Physics();
	m_physics->Init();

	// オーディオを生成する
	m_audio = new Audio();
	m_audio->Init();

	// シーンマネージャーを生成する
	m_sceneManager = new SceneManager();
	m_sceneManager->Init();

	// 入力マネージャーを生成する
	m_inputManager = new InputManager();
	m_inputManager->Init(hInstance, hWnd);

	// リソースデータマネージャーを生成する
	m_resourceDataManager = new ResourceDataManager();

	// フォントを読み込む
	TextUI::InitLoadFont();
}

//=============================================================
// 終了
//=============================================================
void Manager::Uninit()
{
	// すべてのオブジェクトを破棄する
	Object::AllDestroy();

	// リソースデータをすべて破棄する
	if (m_resourceDataManager != nullptr)
	{
		m_resourceDataManager->AllRelease();
		delete m_resourceDataManager;
		m_resourceDataManager = nullptr;
	}

	// 入力マネージャーを終了する
	if (m_inputManager != nullptr)
	{
		m_inputManager->Uninit();
		delete m_inputManager;
		m_inputManager = nullptr;
	}

	// シーンマネージャーを終了する
	if (m_sceneManager != nullptr)
	{
		m_sceneManager->AllRelease();
		delete m_sceneManager;
		m_sceneManager = nullptr;
	}

	// オーディオを終了する
	if (m_audio != nullptr)
	{
		m_audio->Uninit();
		delete m_audio;
		m_audio = nullptr;
	}

	// 物理を終了する
	if (m_physics != nullptr)
	{
		m_physics->Uninit();
		delete m_physics;
		m_physics = nullptr;
	}

	// レンダラーを終了する
	if (m_renderer != nullptr)
	{
		m_renderer->Uninit();
		delete m_renderer;
		m_renderer = nullptr;
	}

#if OUTPUT_LOG_FILE
	// ログを書き出す
	Log::OutputLog();
#endif
}

//=============================================================
// 更新
//=============================================================
void Manager::Update()
{
	// 入力マネージャーを更新する
	m_inputManager->Update();

	// 物理を更新する
	m_physics->Update();

	// オーディオを更新する
	m_audio->Update();

	// ゲームオブジェクトを更新する
	GameObject::AllUpdate();

	// レンダラーを更新する
	m_renderer->Update();

	// シーンを更新する
	m_sceneManager->Update();

	// トランスフォーム監視の更新
	TransformMonitor::AllUpdate();

	// デスフラグがついているオブジェクトを破棄する
	Object::ReleaseDeathFlag();
}

//=============================================================
// 描画
//=============================================================
void Manager::Draw()
{
	// レンダラーを描画する
	m_renderer->Draw();
}
