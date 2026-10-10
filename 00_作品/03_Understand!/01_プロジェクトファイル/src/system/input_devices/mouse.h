//------------------------------------------------------------
// @file		mouse.h
// @brief	入力 / マウス
//------------------------------------------------------------
#ifndef _INPUT_MOUSE_H_
#define _INPUT_MOUSE_H_

//@brief マウスデバイス
class InputMouse : public InputDevice
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
	static constexpr int MAX_MOUSE_KEY = 3;		// マウスのキー数
	bool m_mouseState[MAX_MOUSE_KEY];
};

// キーボード
struct MouseTable
{
	KeyCode key;
};
const MouseTable mouseTable[] =
{
	{ KeyCode::LeftClick },
	{ KeyCode::RightClick },
	{ KeyCode::WheelClick }
};

#endif // !_INPUT_MOUSE_H_
