//------------------------------------------------------------
// @file		input.h
// @brief	入力
// @detail	入力に対するトリガーを提供します
//------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

#include "KeyTables.h"
using namespace InputSystem;

namespace InputSystem
{
	//@brief 入力デバイス（ベース）
	class InputDevice
	{
	public:
		//@brief 初期化
		virtual bool Init(HINSTANCE hInstance, HWND hwnd) = 0;

		//@brief 終了
		virtual void Uninit() = 0;

		//@brief 更新
		virtual void Update() = 0;

		//@brief 入力情報の取得
		virtual bool GetKey(KeyCode keycode) { return false; }
	};


	//@brief 入力マネージャー
	class InputManager final
	{
	public:
		//@brief コンストラクタ
		InputManager();

		//@brief 初期化
		bool Init(HINSTANCE hInstance, HWND hwnd);

		//@brief 終了
		void Uninit();

		//@brief 更新
		void Update();

		//@brief DirectInputを取得する
		LPDIRECTINPUT8 GetInputDevice() { return m_input; }

		//@brief キーが押されているか
		//@details キーが押されている状態であれば、trueを返します
		bool GetKey(KeyCode keycode);

		//@brief キーが押されたか
		//@details キーが押されたフレームのみ、trueを返します
		bool GetKeyDown(KeyCode keycode);

		//@brief キーが離されたか
		//@details キーが離されたフレームのみ、trueを返します
		bool GetKeyUp(KeyCode keycode);

		//@brief コントローラーの左スティックの値を取得します
		D3DXVECTOR2 GetLStickValue();

		//@brief コントローラーの右スティックの値を取得します
		D3DXVECTOR2 GetRStickValue();
	private:
		LPDIRECTINPUT8 m_input;												// 入力
		std::vector<std::unique_ptr<InputDevice>> m_devices;	// 入力デバイス
		std::vector<KeyCode> m_keyData;									// キーデータ
		std::vector<KeyCode> m_oldKeyData;							// 前回のキーデータ
	};
}

#endif // !_INPUT_H_