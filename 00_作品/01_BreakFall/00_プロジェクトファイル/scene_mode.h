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
	SceneModeTitle(Scene* pScene) : SceneMode(pScene), m_oldSelect(0)
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
	Object* pBG;				//背景
	int m_oldSelect;			//前フレームの選択
	//static const float END_HEIGHT = 600.0f;
};

//チュートリアルシーン
class SceneModeTutorial : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	SceneModeTutorial(Scene* pScene) : SceneMode(pScene), m_pTutorial(nullptr), m_page(0)
	{
	}

	//デストラクタ
	~SceneModeTutorial() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
	Object* m_pTutorial;
	int m_page;
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

	static int GetScore() { return s_score; }	//スコアの取得処理
	static void AddScore(int score) { s_score += score; }	//スコアの加算処理

private:
	Pause* m_pPause;   //ポーズ
	Object* m_pScore;	//スコア表示
	DWORD m_timer;		//タイマー
	static int s_score;		//スコア
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
class SceneModeRanking : public SceneMode   //(親)シーンモード
{
public:
	//コンストラクタ
	SceneModeRanking(Scene* pScene) : SceneMode(pScene)
	{
	}

	//デストラクタ
	~SceneModeRanking()
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
