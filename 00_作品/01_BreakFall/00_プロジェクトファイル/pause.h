//========================================================================================================
//
//ポーズの処理[pause.h]
// Auhtor Murata Shinnosuke
//
//========================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "object.h"
#include "menu.h"

//ポーズクラス
class Pause
{
public:
	//コンストラクタ
	Pause()
	{
		//メンバ変数を初期化する
		this->m_isPause = false;
		this->m_pPauseMenu = nullptr;
	}

	//デストラクタ
	~Pause()
	{
	}

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw();   //描画処理
	void SwitchPause();   //ポーズの切り替え処理
	bool GetPause() { return this->m_isPause; };   //ポーズフラグの取得処理
	Menu* GetMenu() { return this->m_pPauseMenu->GetComponent<Menu>(); };   //選択中の選択肢の取得処理

private:
	bool m_isPause;   //ポーズフラグ
	Object* m_pPauseMenu;   //メニュー
};

#endif // !_PAUSE_H_