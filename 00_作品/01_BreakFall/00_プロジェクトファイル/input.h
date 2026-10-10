//========================================================================================================
//
//入力処理[input.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

class Input
{
public:
	Input();   //コンストラクタ
	~Input();   //デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);   //初期化処理
	virtual void Uninit();   //終了処理
	virtual void Update() = 0;   //更新処理

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;   //デバイス
	static LPDIRECTINPUT8 m_pInput;   //入力
};

#endif // !_INPUT_H_
