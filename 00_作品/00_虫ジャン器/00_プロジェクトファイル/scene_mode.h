//========================================================================================================
//
//シーンのモード[scene_mode.h]
// Author MurataShinnosuke
//
//========================================================================================================
#ifndef _SCENE_MODE_H_
#define _SCENE_MODE_H_

#include "main.h"
#include "object.h"
#include "pause.h"

//前方宣言
class Scene;   //シーン
//シーンのモード
class SceneMode
{
public:
	//コンストラクタ
	SceneMode(Scene* pScene)
	{
		//メンバ変数を初期化する
		this->m_pScene = pScene;
	}

	//デストラクタ
	virtual ~SceneMode()
	{
	}

	virtual void Init() = 0;   //初期化処理
	virtual void Uninit() = 0;   //終了処理
	virtual void Update() = 0;   //更新処理
	virtual void Draw() = 0;   //描画処理

protected:
	Scene* GetScene() { return this->m_pScene; };   //シーンの取得

private:
	Scene* m_pScene;   //シーン
};

//タイトルシーン
class SceneModeTitle : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	SceneModeTitle(Scene* pScene) : SceneMode(pScene)
	{
		//メンバ変数を初期化する
		this->pTitleMenu = nullptr;
	}

	//デストラクタ
	~SceneModeTitle() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
	Object* pTitleMenu;   //タイトルメニュー
};

//ゲームシーン
class SceneModeGame : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	SceneModeGame(Scene* pScene) : SceneMode(pScene)
	{
		//メンバ変数を初期化する
		this->m_pPause = nullptr;   //ポーズ
	}

	//デストラクタ
	~SceneModeGame() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

	Pause* GetPause() { return this->m_pPause; };   //ポーズの取得処理

private:
	Pause* m_pPause;   //ポーズ
};

//リザルトシーン
class SceneModeResult : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	SceneModeResult(Scene* pScene) : SceneMode(pScene)
	{
	}

	//デストラクタ
	~SceneModeResult() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
};

//ランキングシーンクラス
class Ranking : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	Ranking(Scene* pScene) : SceneMode(pScene)
	{
	}

	//デストラクタ
	~Ranking()
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
	void Control();   //操作処理
};


#endif // !_SCENE_MODE_H_
