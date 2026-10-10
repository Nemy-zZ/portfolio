//========================================================================================================
//
//ポーズメニューの処理[pause_menu.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "behavior.h"

//ポーズメニュークラス
class PauseMenu : public Behavior   //(親)挙動
{
public:
	const static D3DXVECTOR2 SIZE_MENU;

	typedef enum
	{
		IS_CONTINUE = 0,   //コンテニュー
		IS_REPLAY,   //リプレイ
		IS_QUIT,   //終了
		IS_MAX
	}MENU;   //メニュー

	//コンストラクタ
	PauseMenu(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~PauseMenu()
	{
	}

	void Init();   //初期化
	void Update();   //更新処理

private:
};

#endif // !_PAUSE_MENU_H_