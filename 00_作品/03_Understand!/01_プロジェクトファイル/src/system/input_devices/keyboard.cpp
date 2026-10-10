//------------------------------------------------------------
// @file		keyboard.cpp
// @brief	入力 / キーボード
//------------------------------------------------------------
#include "keyboard.h"
#include "system/manager.h"

//=============================================================
// 初期化
//=============================================================
bool InputKeyboard::Init(HINSTANCE hInstance, HWND hwnd)
{
	// 入力の取得
	LPDIRECTINPUT8 inputDevice = Manager::GetInstance()->GetInputManager()->GetInputDevice();

	// 入力デバイス: キーボードの生成
	if (FAILED(inputDevice->CreateDevice(GUID_SysKeyboard, &m_device, nullptr)))
	{
		return false;
	}

	// データフォーマットを設定
	if (FAILED(m_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	// 協調モードを設定
	if (FAILED(m_device->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return false;
	}

	// キーボードへのアクセス権を獲得
	m_device->Acquire();

	return S_OK;
}

//=============================================================
// 終了
//=============================================================
void InputKeyboard::Uninit()
{
	// デバイスの破棄
	if (m_device != nullptr)
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = nullptr;
	}
}

//=============================================================
// 更新
//=============================================================
void InputKeyboard::Update()
{
	BYTE keyState[MAX_KEYBOARD_KEY];

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_device->GetDeviceState(sizeof(keyState), &keyState[0])))
	{
		for (int i = 0; i < MAX_KEYBOARD_KEY; i++)
		{
			m_keyState[i] = keyState[i];
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得
		m_device->Acquire();
	}
}

//=============================================================
// 入力情報の取得
//=============================================================
bool InputKeyboard::GetKey(KeyCode keycode)
{
	// キーボード用キーテーブルに含まれているか
	BYTE code;
	for (int i = 0; i < _countof(keyboardTable); i++)
	{
		if (keyboardTable[i].key == keycode)
		{ // キーコードが一致するとき
			code = keyboardTable[i].code;
			return (m_keyState[code] & 0x80) != 0;
		}
	}
	return false;
}
