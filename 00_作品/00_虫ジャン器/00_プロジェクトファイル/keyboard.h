//********************************************************************************************************
//
//キーボード入力処理[input_keyboard.h]
// Author Murata Shinnosuke
//
//********************************************************************************************************
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "input.h"

class Keyboard : public Input
{
public:
	static const int MAX = 256;   //キーの数

	Keyboard();   //コンストラクタ
	~Keyboard();  //デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	bool GetPress(int nKey);   //プレス情報の取得処理
	bool GetTrigger(int nKey);   //トリガー情報の取得処理

private:
	BYTE m_aState[MAX];   //状態
	BYTE m_aStateTrigger[MAX];   //トリガー状態
};

#endif // !_KEYBOARD_H_
