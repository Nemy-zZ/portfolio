//========================================================================================================
//
//ジョイパッド入力処理[input_joypad.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "input.h"

//ジョイパッドクラス
class Joypad : public Input
{
public:
	static const int MAX_NUM = 4;   //ジョイパッドの最大数

	typedef enum
	{
		IS_NULL = 0,
		IS_UP = XINPUT_GAMEPAD_DPAD_UP,
		IS_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		IS_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		IS_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		IS_START = XINPUT_GAMEPAD_START,
		IS_BACK = XINPUT_GAMEPAD_BACK,
		IS_LT = XINPUT_GAMEPAD_LEFT_THUMB,
		IS_RT = XINPUT_GAMEPAD_RIGHT_THUMB,
		IS_LS = XINPUT_GAMEPAD_LEFT_SHOULDER,
		IS_RS = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		IS_A = XINPUT_GAMEPAD_A,
		IS_B = XINPUT_GAMEPAD_B,
		IS_X = XINPUT_GAMEPAD_X,
		IS_Y = XINPUT_GAMEPAD_Y
	}JOYKEY;   //ジョイパッド

	Joypad();   //コンストラクタ
	~Joypad();  //デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	bool GetPress(JOYKEY key, int nId);   //プレス情報の取得処理
	bool GetTrigger(JOYKEY key, int nId);   //トリガー情報の取得処理
	XINPUT_STATE* GetState(int nId);   //ジョイパッド状態の取得処理

private:
	XINPUT_STATE m_aState[MAX_NUM];   //状態
	XINPUT_STATE m_aStateTrigger[MAX_NUM];   //トリガー状態
	bool m_isButtonPressed[MAX_NUM];   //押されたかどうか
};

#endif // !_JOYPAD_H_
