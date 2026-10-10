//========================================================================================================
//
//レンダラー処理[renderer.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//レンダラークラス
class Renderer
{
public:
	Renderer();   //コンストラクタ
	~Renderer();   //デストラクタ

	HRESULT Init(HWND hWnd, BOOL bWindow);   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw();   //描画処理
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; };   //デバイスの取得処理
	HWND GetHWND() { return m_hWnd; };   //ウィンドウハンドルの取得処理
	void SetFullScreen(bool bFullScreen);	// フルスクリーン

private:
	LPDIRECT3D9 m_pD3D;   //Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;   //Direct3Dデバイス
	HWND m_hWnd;
	LPD3DXFONT m_pFont;   //フォント
	bool m_isFullScreen;   //フルスクリーンフラグ
};

#endif // !_RENDERER_H_
