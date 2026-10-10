//------------------------------------------------------------
// @file		keyboard.h
// @brief	入力 / キーボード
//------------------------------------------------------------
#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_

#include "system/input.h"

//@brief キーボードデバイス
class InputKeyboard : public InputDevice
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
private:
	static constexpr int MAX_KEYBOARD_KEY = 256;		// キーボードのキー数

	BYTE m_keyState[MAX_KEYBOARD_KEY];
	LPDIRECTINPUTDEVICE8 m_device;
};

// キーボード
struct KeyboardTable
{
	KeyCode key;
	BYTE code;
};
const KeyboardTable keyboardTable[] =
{
	{ KeyCode::Escape, DIK_ESCAPE },
	{ KeyCode::Alpha1, DIK_1 },
	{ KeyCode::Alpha2, DIK_2 },
	{ KeyCode::Alpha3, DIK_3 },
	{ KeyCode::Alpha4, DIK_4 },
	{ KeyCode::Alpha5, DIK_5 },
	{ KeyCode::Alpha6, DIK_6 },
	{ KeyCode::Alpha7, DIK_7 },
	{ KeyCode::Alpha8, DIK_8 },
	{ KeyCode::Alpha9, DIK_9 },
	{ KeyCode::Alpha0, DIK_0 },
	{ KeyCode::Minus, DIK_MINUS },
	{ KeyCode::Equals, DIK_EQUALS },
	{ KeyCode::Backspace, DIK_BACK },
	{ KeyCode::Tab, DIK_TAB },
	{ KeyCode::Q, DIK_Q },
	{ KeyCode::W, DIK_W },
	{ KeyCode::E, DIK_E },
	{ KeyCode::R, DIK_R },
	{ KeyCode::T, DIK_T },
	{ KeyCode::Y, DIK_Y },
	{ KeyCode::U, DIK_U },
	{ KeyCode::I, DIK_I },
	{ KeyCode::O, DIK_O },
	{ KeyCode::P, DIK_P },
	{ KeyCode::LeftBracket, DIK_LBRACKET },
	{ KeyCode::RightBracket, DIK_RBRACKET },
	{ KeyCode::Return, DIK_RETURN },
	{ KeyCode::LeftControl, DIK_LCONTROL },
	{ KeyCode::A, DIK_A },
	{ KeyCode::S, DIK_S },
	{ KeyCode::D, DIK_D },
	{ KeyCode::F, DIK_F },
	{ KeyCode::G, DIK_G },
	{ KeyCode::H, DIK_H },
	{ KeyCode::J, DIK_J },
	{ KeyCode::K, DIK_K },
	{ KeyCode::L, DIK_L },
	{ KeyCode::Semicolon, DIK_SEMICOLON },
	{ KeyCode::Apostrophe, DIK_APOSTROPHE },
	{ KeyCode::BackQuote, DIK_GRAVE },
	{ KeyCode::LeftShift, DIK_LSHIFT },
	{ KeyCode::Backslash, DIK_BACKSLASH },
	{ KeyCode::Z, DIK_Z },
	{ KeyCode::X, DIK_X },
	{ KeyCode::C, DIK_C },
	{ KeyCode::V, DIK_V },
	{ KeyCode::B, DIK_B },
	{ KeyCode::N, DIK_N },
	{ KeyCode::M, DIK_M },
	{ KeyCode::Comma, DIK_COMMA },
	{ KeyCode::Period, DIK_PERIOD },
	{ KeyCode::Slash, DIK_SLASH },
	{ KeyCode::RightShift, DIK_RSHIFT },
	{ KeyCode::Asterisk, DIK_MULTIPLY },
	{ KeyCode::LeftAlt, DIK_LALT },
	{ KeyCode::LeftMenu, DIK_LMENU },
	{ KeyCode::Space, DIK_SPACE },
	{ KeyCode::CapsLock, DIK_CAPITAL },
	{ KeyCode::F1, DIK_F1 },
	{ KeyCode::F2, DIK_F2 },
	{ KeyCode::F3, DIK_F3 },
	{ KeyCode::F4, DIK_F4 },
	{ KeyCode::F5, DIK_F5 },
	{ KeyCode::F6, DIK_F6 },
	{ KeyCode::F7, DIK_F7 },
	{ KeyCode::F8, DIK_F8 },
	{ KeyCode::F9, DIK_F9 },
	{ KeyCode::F10, DIK_F10 },
	{ KeyCode::Numlock, DIK_NUMLOCK },
	{ KeyCode::ScrollLock, DIK_SCROLL },
	{ KeyCode::Keypad7, DIK_NUMPAD7 },
	{ KeyCode::Keypad8, DIK_NUMPAD8 },
	{ KeyCode::Keypad9, DIK_NUMPAD9 },
	{ KeyCode::KeypadMinus, DIK_SUBTRACT },
	{ KeyCode::Keypad4, DIK_NUMPAD4 },
	{ KeyCode::Keypad5, DIK_NUMPAD5 },
	{ KeyCode::Keypad6, DIK_NUMPAD6 },
	{ KeyCode::KeypadPlus, DIK_ADD },
	{ KeyCode::Keypad1, DIK_NUMPAD1 },
	{ KeyCode::Keypad2, DIK_NUMPAD2 },
	{ KeyCode::Keypad3, DIK_NUMPAD3 },
	{ KeyCode::Keypad0, DIK_NUMPAD0 },
	{ KeyCode::KeypadPeriod, DIK_DECIMAL },
	{ KeyCode::F11, DIK_F11 },
	{ KeyCode::F12, DIK_F12 },
	{ KeyCode::F13, DIK_F13 },
	{ KeyCode::F14, DIK_F14 },
	{ KeyCode::F15, DIK_F15 },
	{ KeyCode::Kana, DIK_KANA },
	{ KeyCode::Convert, DIK_CONVERT },
	{ KeyCode::NoConvert, DIK_NOCONVERT },
	{ KeyCode::Yen, DIK_YEN },
	{ KeyCode::KeypadEquals, DIK_NUMPADEQUALS },
	{ KeyCode::Caret, DIK_CIRCUMFLEX },
	{ KeyCode::At, DIK_AT },
	{ KeyCode::Colon, DIK_COLON },
	{ KeyCode::Underscore, DIK_UNDERLINE },
	{ KeyCode::Kanji, DIK_KANJI },
	{ KeyCode::Stop, DIK_STOP },
	{ KeyCode::KeypadEnter, DIK_NUMPADENTER },
	{ KeyCode::RightControl, DIK_RCONTROL },
	{ KeyCode::KeypadDivide, DIK_DIVIDE },
	{ KeyCode::RightAlt, DIK_RALT },
	{ KeyCode::RightMenu, DIK_RMENU },
	{ KeyCode::Pause, DIK_PAUSE },
	{ KeyCode::Home, DIK_HOME },
	{ KeyCode::UpArrow, DIK_UP },
	{ KeyCode::PageUp, DIK_PRIOR },
	{ KeyCode::LeftArrow, DIK_LEFT },
	{ KeyCode::RightArrow, DIK_RIGHT },
	{ KeyCode::End, DIK_END },
	{ KeyCode::DownArrow, DIK_DOWN },
	{ KeyCode::PageDown, DIK_NEXT },
	{ KeyCode::Insert, DIK_INSERT },
	{ KeyCode::Delete, DIK_DELETE },
	{ KeyCode::LeftWindows , DIK_LWIN },
	{ KeyCode::RightWindows, DIK_RWIN },
	{ KeyCode::Menu, DIK_APPS },
	{ KeyCode::Power, DIK_POWER },
	{ KeyCode::Sleep, DIK_SLEEP },
};

#endif // !_INPUT_KEYBOARD_H_
