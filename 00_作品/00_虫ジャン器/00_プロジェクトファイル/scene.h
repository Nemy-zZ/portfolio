//========================================================================================================
//
//シーン処理[scene.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "scene_mode.h"

//シーンクラス
class Scene
{
public:
	//モードの列挙
	enum MODE
	{
		IS_TITLE = 0,   //タイトル
		IS_GAME,   //ゲーム
		IS_RESULT,   //リザルト
		IS_RANKIGN,   //ランキング
	};

	//コンストラクタ
	Scene()
	{
		//メンバ変数を初期化する
		this->m_isNext = false;   //遷移フラグ
		this->m_modeNext = Scene::MODE::IS_TITLE;   //次のモード
		this->m_pMode = nullptr;   //モード
	}

	//デストラクタ
	~Scene()
	{
		//モードを破棄する
		delete this->m_pMode;
	}

	void Init() { this->m_pMode->Init(); };   //初期化処理
	void Uninit() { this->m_pMode->Uninit(); };   //終了処理
	void Update() { this->m_pMode->Update(); };   //更新処理
	void Draw() { this->m_pMode->Draw(); };   //描画処理
	void ChangeMode(SceneMode* pMode);   //モードの変更
	void Reset() { this->m_isNext = false; };   //リセット
	void Next(Scene::MODE modeNext) { this->m_isNext = true; this->m_modeNext = modeNext; };   //遷移フラグを立てる
	bool GetNext() { return this->m_isNext; };   //遷移フラグの取得
	Scene::MODE GetNextMode() { return this->m_modeNext; };   //次のモードの取得

private:
	bool m_isNext;   //遷移フラグ
	Scene::MODE m_modeNext;   //次のモード
	SceneMode* m_pMode;   //モード
};

#endif // !_SCENE_H_
