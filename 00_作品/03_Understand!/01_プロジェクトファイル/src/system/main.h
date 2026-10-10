//------------------------------------------------------------
// @file		main.h
// @brief	メイン処理
// @detail	ウィンドウの作成やメインループ処理を行います
//------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

 // マクロ定義
#define CLASS_NAME			"WindowClass"		// クラス名
#define WINDOW_NAME	"Understand! ~黒と白の平行線~"					// ウィンドウ名

// インクルード
#include <thread>

//@brief ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//@brief メインクラス
class Main final
{
public:
	~Main();

	//@brief インスタンスを取得する
	static Main& GetInstance()
	{
		static Main instance;
		return instance;
	}

	//@brief 開始
	void Start();

	//@brief メインループ
	void MainLoop();

	//@brief 起動時刻を取得する
	DWORD GetStartupTime() { return m_startupTime; }
	//@brief 経過時間を取得する
	DWORD GetElapsedTime() { return m_currentTime - m_startupTime; }
	//@brief デルタタイムを取得する
	float GetDeltaTime() { return m_deltaTime; }
	//@brief FPS値を取得する
	int GetFPS() { return m_fps; }
	//@brief ウィンドウのアクティブ状態を取得する
	bool IsActiveWindow();
	//@brief アプリケーションを終了する
	void ExitApplication();
	//@brief カーソルの表示状態を変更する
	void SetShowCursor(const bool& show);

	//@brief カーソル位置
	struct CursorPos
	{
		long x;
		long y;
	};

	//@brief クライアント内のカーソル位置を取得する
	CursorPos GetCursorClientPos();
	//@brief クライアント内のカーソル位置を設定する
	void SetCursorClientPos(long x, long y);
	//@brief カーソルの移動量
	D3DXVECTOR2 GetMoveCursor();

	//@brief ウィンドウサイズを取得する
	D3DXVECTOR2 GetWindowSize();
	//@brief ウィンドウハンドルを取得する
	HWND GetHWND();
private:
	//@brief コンストラクタ
	Main();

	std::thread* m_thread;		// スレッド
	DWORD m_currentTime;	// 現在時刻
	DWORD m_execLastTime;	// 前回の更新時刻
	DWORD m_fpsLastTime;		// 前回のFPS更新時刻
	DWORD m_startupTime;		// 起動時刻

	int m_frameCount;				// フレームのカウント

	float m_deltaTime;				// デルタタイム
	int m_fps;							// FPS値

	D3DXVECTOR2 m_moveCursor;
	D3DXVECTOR2 m_oldMoveCursor;
};

#endif // !_MAIN_H_