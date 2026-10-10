//========================================================================================================
//
//メニューの処理[menu.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "rendering.h"
#include "select_box.h"

//メニュークラス
class Menu : public Rendering   //(親)レンダリング
{
public:
	//コンストラクタ
	Menu(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_vSelectBox.clear();
		this->m_nSelect = 0;
		this->m_fGap = 0.0f;   //選択肢の隙間
		this->m_isDecide = false;
	}

	//デストラクタ
	~Menu() override
	{
	}

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw();   //描画処理

	void SetSelectBox(int nNum);   //選択肢の設定処理
	void SetGap(float fGap) { this->m_fGap = fGap; };   //選択肢の隙間の設定処理

	bool GetDecide() { return this->m_isDecide; };   //決定フラグの取得処理
	int GetSelect() { return this->m_nSelect; };   //選択中の選択肢

private:
	void Control();   //操作処理

	std::vector<SelectBox*> m_vSelectBox;   //選択肢
	int m_nSelect;   //選択中の選択肢
	float m_fGap;   //選択肢の隙間
	bool m_isDecide;   //決定フラグ
};

#endif // !_MENU_H_