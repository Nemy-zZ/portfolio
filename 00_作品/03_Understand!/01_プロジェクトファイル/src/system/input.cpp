//------------------------------------------------------------
// @file		input.cpp
// @brief	入力
// @detail	入力に対するトリガーを提供します
//------------------------------------------------------------
#include "input.h"

#include "input_devices/keyboard.h"
#include "input_devices/mouse.h"
#include "input_devices/gamepad.h"

//=============================================================
// コンストラクタ
//=============================================================
InputManager::InputManager()
	: m_input(nullptr)
{
	m_devices.emplace_back(new InputKeyboard());
	m_devices.emplace_back(new InputMouse());
	m_devices.emplace_back(new InputGamepad());
}

//=============================================================
// 初期化
//=============================================================
bool InputManager::Init(HINSTANCE hInstance, HWND hwnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_input, nullptr)))
	{
		Log::SendLog("入力オブジェクトの生成に失敗しました（DirectInput8）", Log::TYPE_ERROR);
		return false;
	}

	// デバイスを初期化する
	for (auto itr = m_devices.begin(); itr != m_devices.end(); itr++)
	{
		(**itr).Init(hInstance, hwnd);
	}

	return true;
}

//=============================================================
// 終了
//=============================================================
void InputManager::Uninit()
{
	// DirectInputオブジェクトの破棄
	if (m_input != nullptr)
	{
		m_input->Release();
		m_input = nullptr;
	}

	// デバイスを初期化する
	for (auto itr = m_devices.begin(); itr != m_devices.end(); itr++)
	{
		(**itr).Uninit();
	}
	m_devices.clear();
}

//=============================================================
// 更新
//=============================================================
void InputManager::Update()
{
	// 前回の入力データにデータを移動する
	m_oldKeyData = std::move(m_keyData);
	m_keyData.clear();

	// デバイスの更新
	for (auto itr = m_devices.begin(); itr != m_devices.end(); itr++)
	{
		(**itr).Update();
	}

	// 入力されているキーコード
	for (auto itr = m_devices.begin(); itr != m_devices.end(); itr++)
	{
		for (int i = 0; i < KeyCode::MAX; i++)
		{
			// 対象のキーコード
			KeyCode keycode = static_cast<KeyCode>(i);

			// すでに入力されているとき
			if (std::find(m_keyData.begin(), m_keyData.end(), keycode) != m_keyData.end())
			{
				continue;
			}

			// 入力されているとき
			if ((**itr).GetKey(keycode))
			{
				m_keyData.emplace_back(keycode);
			}
		}
	}
}

//=============================================================
// キーが押されているか
//=============================================================
bool InputSystem::InputManager::GetKey(KeyCode keycode)
{
	return std::find(m_keyData.begin(), m_keyData.end(), keycode) != m_keyData.end();
}

//=============================================================
// キーが押されたか
//=============================================================
bool InputSystem::InputManager::GetKeyDown(KeyCode keycode)
{
	return std::find(m_keyData.begin(), m_keyData.end(), keycode) != m_keyData.end() &&
		std::find(m_oldKeyData.begin(), m_oldKeyData.end(), keycode) == m_oldKeyData.end();
}

//=============================================================
// キーが離されたか
//=============================================================
bool InputSystem::InputManager::GetKeyUp(KeyCode keycode)
{
	return std::find(m_keyData.begin(), m_keyData.end(), keycode) == m_keyData.end() &&
		std::find(m_oldKeyData.begin(), m_oldKeyData.end(), keycode) != m_oldKeyData.end();
}

//=============================================================
// 左スティックの値を取得する
//=============================================================
D3DXVECTOR2 InputSystem::InputManager::GetLStickValue()
{
	InputGamepad* gamepad = (InputGamepad*)m_devices[2].get();
	return D3DXVECTOR2((float)gamepad->GetLStickX(), (float)gamepad->GetLStickY());
}

//=============================================================
// 右スティックの値を取得する
//=============================================================
D3DXVECTOR2 InputSystem::InputManager::GetRStickValue()
{
	InputGamepad* gamepad = (InputGamepad*)m_devices[2].get();
	return D3DXVECTOR2((float)gamepad->GetRStickX(), (float)gamepad->GetRStickY());
}
