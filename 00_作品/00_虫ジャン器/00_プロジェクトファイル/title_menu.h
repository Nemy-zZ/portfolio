//========================================================================================================
//
//タイトルメニューの処理[title_menu.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _TITLE_MENU_
#define _TITLE_MENU_

#include "behavior.h"

//タイトルメニュークラス
class TitleMenu : public Behavior   //(親)挙動
{
public:
	const static D3DXVECTOR2 SIZE_MENU;   //メニューのサイズ

	typedef enum
	{
		IS_START = 0,
		IS_RANKING,
	    IS_MAX
	}MENU;   //メニュー

	//コンストラクタ
	TitleMenu(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~TitleMenu()
	{
	}

	void Init() override;   //初期化
	void Update() override;   //更新処理

private:
};

#endif // !_TITLE_MENU_