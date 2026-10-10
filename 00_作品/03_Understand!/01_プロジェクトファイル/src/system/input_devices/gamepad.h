//------------------------------------------------------------
// @file		gamepad.h
// @brief	入力 / ゲームパッド
//------------------------------------------------------------
#ifndef _INPUT_GAMEPAD_H_
#define _INPUT_GAMEPAD_H_

//@brief ゲームパッドデバイス
class InputGamepad : public InputDevice
{
public:
	//@brief 初期化
	bool Init(HINSTANCE hInstance, HWND hwnd) override;

	//@brief 終了
	void Uninit() override;

	//@brief 更新
	void Update() override;

	//@brief 入力情報の取得
	bool GetKey(KeyCode keycode) override;

	//@brief 左スティックXの取得
	short GetLStickX();
	//@brief 左スティックYの取得
	short GetLStickY();

	//@brief 右スティックXの取得
	short GetRStickX();
	//@brief 右スティックYの取得
	short GetRStickY();
private:
	XINPUT_STATE m_gamepadState;
};

// ゲームパッド
struct GamepadTable
{
	KeyCode key;
	WORD code;
};
const GamepadTable gamepadTable[] =
{
	{ KeyCode::PadUp, XINPUT_GAMEPAD_DPAD_UP },
	{ KeyCode::PadDown, XINPUT_GAMEPAD_DPAD_DOWN },
	{ KeyCode::PadLeft, XINPUT_GAMEPAD_DPAD_LEFT },
	{ KeyCode::PadRight, XINPUT_GAMEPAD_DPAD_RIGHT },
	{ KeyCode::PadStart, XINPUT_GAMEPAD_START },
	{ KeyCode::PadBack, XINPUT_GAMEPAD_BACK },
	{ KeyCode::PadLT, XINPUT_GAMEPAD_LEFT_THUMB },
	{ KeyCode::PadRT, XINPUT_GAMEPAD_RIGHT_THUMB },
	{ KeyCode::PadLS, XINPUT_GAMEPAD_LEFT_SHOULDER },
	{ KeyCode::PadRS, XINPUT_GAMEPAD_RIGHT_SHOULDER },
	{ KeyCode::PadA, XINPUT_GAMEPAD_A },
	{ KeyCode::PadB, XINPUT_GAMEPAD_B },
	{ KeyCode::PadX, XINPUT_GAMEPAD_X },
	{ KeyCode::PadY, XINPUT_GAMEPAD_Y }
};
struct GamepadOtherTable
{
	KeyCode key;
};
const GamepadTable gamepadOtherTable[] =
{
	{ KeyCode::PadLStickUp },
	{ KeyCode::PadLStickDown },
	{ KeyCode::PadLStickLeft },
	{ KeyCode::PadLStickRight },
	{ KeyCode::PadRStickUp },
	{ KeyCode::PadRStickDown },
	{ KeyCode::PadRStickLeft },
	{ KeyCode::PadRStickRight },
	{ KeyCode::PadLTrigger },
	{ KeyCode::PadRTrigger },
};

#endif // !_INPUT_GAMEPAD_H_
