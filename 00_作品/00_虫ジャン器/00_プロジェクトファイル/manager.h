//========================================================================================================
//
//マネージャー処理[manager.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "benlib.h"
#include "renderer.h"
#include "texture.h"
#include "Xfile.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "scene.h"
#include "stage.h"

//マネージャークラス
class Manager
{
public:
	//デストラクタ
	~Manager()
	{
	}

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);   //初期化
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw();   //描画処理
	Renderer* GetRenderer() { return this->m_pRenderer; };   //レンダラーの取得処理
	Texture* GetTexture() { return this->m_pTexture; };   //テクスチャの取得処理
	XFile* GetXFile() { return this->m_pXFile; };   //Xファイルの取得処理
	Keyboard* GetKeyboard() { return this->m_pKeyboard; };   //キーボードの取得処理
	Mouse* GetMouse() { return this->m_pMouse; };   //マウスの取得処理
	Joypad* GetJoypad() { return this->m_pJoypad; };   //ジョイパッドの取得処理
	Scene* GetScene() { return this->m_pScene; };   //シーンの取得処理
	Stage* GetStage() { return this->m_pStage; };   //ステージの取得

	//生成処理
	static void Create()
	{
		//インスタンスを確認する
		if (Manager::m_pInstance != nullptr)
		{
			//処理終了
			return;
		}

		//インスタンスを生成する
		Manager::m_pInstance = new Manager;
	}

	static Manager* Get() { return Manager::m_pInstance; };   //マネージャの取得

private:
	//コンストラクタ
	Manager()
	{
		//メンバ変数を初期化する
		this->m_pRenderer = nullptr;
		this->m_pTexture = nullptr;
		this->m_pXFile = nullptr;
		this->m_pKeyboard = nullptr;
		this->m_pMouse = nullptr;
		this->m_pJoypad = nullptr;
		this->m_pScene = nullptr;
		this->m_pStage = nullptr;
	}

	Renderer* m_pRenderer;   //レンダラー
	Texture* m_pTexture;   //テクスチャ
	XFile* m_pXFile;   //Xファイル
	Keyboard* m_pKeyboard;   //キーボード
	Mouse* m_pMouse;   //マウス
	Joypad* m_pJoypad;   //ジョイパッド
	Scene* m_pScene;   //シーン
	Stage* m_pStage;   //ステージ

	static Manager* m_pInstance;   //インスタンス
};

#endif // !_MANAGER_H_
