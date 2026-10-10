//===========================================================================================================
//
//入力アクションの処理[input_action.h]
// Author Murata Shinnosuke
//
//===========================================================================================================
#ifndef _INPUT_ACTION_H_
#define _INPUT_ACTION_H_

#include "main.h"

//入力アクション
class InputAction
{
public:
	//コンストラクタ
	InputAction(std::string sName)
	{
		//メンバ変数を初期化する
		this->m_sName = sName;
	}

	//デストラクタ
	~InputAction()
	{
	}

	void Init();   //初期化処理
	void Uninit();   //終了処理

	static InputAction* Get(int nId) { return InputAction::m_vInputAction[nId]; };   //取得

private:
	std::string m_sName;   //名前

	static std::vector<InputAction*> m_vInputAction;
};

#endif // !_INPUT_ACTION_H_
