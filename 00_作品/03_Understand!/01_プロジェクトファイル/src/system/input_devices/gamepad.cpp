//------------------------------------------------------------
// @file		gamepad.h
// @brief	入力 / ゲームパッド
//------------------------------------------------------------
#include "gamepad.h"

//=============================================================
// 初期化
//=============================================================
bool InputGamepad::Init(HINSTANCE hInstance, HWND hwnd)
{
	// メモリのクリア
	memset(&m_gamepadState, 0, sizeof(XINPUT_STATE));

	// XInputのステートを設定
	XInputEnable(true);

	return true;
}

//=============================================================
// 終了
//=============================================================
void InputGamepad::Uninit()
{
	// XInputのステートを設定
	XInputEnable(false);
}

//=============================================================
// 更新
//=============================================================
void InputGamepad::Update()
{
	// ジョイパッドの情報を取得
	XINPUT_STATE state;
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		m_gamepadState = state;
	}
}

//=============================================================
// 入力情報の取得
//=============================================================
bool InputGamepad::GetKey(KeyCode keycode)
{
	// ボタン
	for (int i = 0; i < _countof(gamepadTable); i++)
	{
		if (gamepadTable[i].key == keycode)
		{ // キーコードが一致するとき
			return (m_gamepadState.Gamepad.wButtons & gamepadTable[i].code) ? true : false;
		}
	}

	// スティック
	for (int i = 0; i < _countof(gamepadOtherTable); i++)
	{
		if (gamepadOtherTable[i].key == keycode)
		{ // キーコードが一致するとき
			short stickL[2] = { 0, 0 };
			short stickR[2] = { 0, 0 };
			stickL[0] = m_gamepadState.Gamepad.sThumbLX;
			stickL[1] = m_gamepadState.Gamepad.sThumbLY;
			stickR[0] = m_gamepadState.Gamepad.sThumbRX;
			stickR[1] = m_gamepadState.Gamepad.sThumbRY;

			switch (keycode)
			{
			case InputSystem::PadLStickUp:
				return (stickL[1] > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			case InputSystem::PadLStickDown:
				return (stickL[1] < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			case InputSystem::PadLStickLeft:
				return (stickL[0] < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			case InputSystem::PadLStickRight:
				return (stickL[0] > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
			case InputSystem::PadRStickUp:
				return (stickR[1] > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			case InputSystem::PadRStickDown:
				return (stickR[1] < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			case InputSystem::PadRStickLeft:
				return (stickR[0] < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			case InputSystem::PadRStickRight:
				return (stickR[0] > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
			case InputSystem::PadLTrigger:
				return (m_gamepadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
			case InputSystem::PadRTrigger:
				return (m_gamepadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
			}
		}
	}

	return false;
}

short InputGamepad::GetLStickX()
{
	if (m_gamepadState.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		m_gamepadState.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return m_gamepadState.Gamepad.sThumbLX;
	}
	return 0;
}

short InputGamepad::GetLStickY()
{
	if (m_gamepadState.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		m_gamepadState.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return m_gamepadState.Gamepad.sThumbLY;
	}
	return 0;
}

short InputGamepad::GetRStickX()
{
	if (m_gamepadState.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		m_gamepadState.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return m_gamepadState.Gamepad.sThumbRX;
	}
	return 0;
}

short InputGamepad::GetRStickY()
{
	if (m_gamepadState.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		m_gamepadState.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return m_gamepadState.Gamepad.sThumbRY;
	}
	return 0;
}
