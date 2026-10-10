//------------------------------------------------------------
// @file		main.cpp
// @brief	メイン処理
// @detail	ウィンドウの作成やメインループ処理を行います
//------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderer.h"

 // メモリーリーク検出
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define FIND_MEM_LEAKS
#endif
#ifdef FIND_MEM_LEAKS
#define _NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define _NEW new
#endif

// グローバル変数の初期化
bool g_isActiveWindow = true;		// ウィンドウがアクティブ状態か
bool g_isEnded = false;					// 終了状態か
bool g_showCursor = true;				// カーソルの表示状態
bool g_beforeShowCursor = true;	// 前回のカーソル表示状態
D3DXVECTOR2 g_cursorMove;		// カーソルの移動量
HWND g_hwnd = nullptr;				// ウィンドウハンドル（識別子）

 //=============================================================
 // メイン関数
 //=============================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// メモリーリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1767);
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// メモリサイズ
		CS_CLASSDC,									// ウィンドウのスタイル
		WindowProc,										// ウィンドウのプロシージャ
		0,
		0,
		hInstance,											// インスタンスハンドル
		LoadIcon(nullptr, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		nullptr,												// メニューバー
		CLASS_NAME,									// ウィンドウクラスの名前
		LoadIcon(nullptr, IDI_APPLICATION)		// ファイルのアイコン
	};

	MSG msg;			// メッセージを格納する変数

	// ウィンドウの座標を格納
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	g_hwnd = CreateWindowEx(
		0,															// 拡張ウィンドウスタイル
		CLASS_NAME,										// ウィンドウクラスの名前
		WINDOW_NAME,								// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,				// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウの左上のX座標
		CW_USEDEFAULT,								// ウィンドウの左上のY座標
		(rect.right - rect.left),							// ウィンドウの幅
		(rect.bottom - rect.top),						// ウィンドウの高さ
		nullptr,													// 親ウィンドウのハンドル
		nullptr,													// メニューハンドルまたは子ウィンドウID
		hInstance,											// インスタンスバンドル
		nullptr													// ウィンドウ作成データ
	);

	// マウスを登録する
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;      // Generic desktop controls
	rid.usUsage = 0x02;      // Mouse
	rid.dwFlags = RIDEV_INPUTSINK; // フォーカスがなくても受け取るならRIDEV_INPUTSINK
	rid.hwndTarget = g_hwnd;
	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
	{
		MessageBox(g_hwnd, "RawInputの登録に失敗しました", "エラー", MB_ICONERROR);
		Main::GetInstance().ExitApplication();
	}

	// ウィンドウの表示
	ShowWindow(g_hwnd, nCmdShow);		// ウィンドウの表示状態を設定
	UpdateWindow(g_hwnd);						// クライアント領域を更新

	// マネージャーの初期化
	Manager::GetInstance()->Init(hInstance, g_hwnd);

	// Mainの取得
	Main& mainApp = Main::GetInstance();

	// 開始
	mainApp.Start();

	// メッセージループ
	while (!g_isEnded)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{ // Windowsの処理
			if (msg.message == WM_QUIT)
			{ // WM_QUITメッセージ
				g_isEnded = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// メインループ処理
			mainApp.MainLoop();

			// アクティブ情報の更新
			g_isActiveWindow = GetActiveWindow() != NULL;

			// カーソルの表示設定の更新
			if (g_showCursor != g_beforeShowCursor)
			{
				if (g_showCursor)
				{
					while (ShowCursor(TRUE) < 0) {}
				}
				else
				{
					while (ShowCursor(FALSE) >= 0) {}
				}

				g_beforeShowCursor = g_showCursor;
			}
		}
	}

	// メインループの終了ログ
	Log::SendLog("メインループを終了しました");

	// マネージャーの終了
	Manager::GetInstance()->Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================
// ウィンドウプロシージャ
//=============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto hdc = GetDC(hWnd);

	UINT dwSize = 0;
	std::vector<BYTE> lpb;
	RAWINPUT* raw;
	D3DXVECTOR2 rect = Main::GetInstance().GetWindowSize();

	int nID;

	switch (uMsg)
	{
	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "確認", MB_YESNO); //終了メッセージ
			if (nID == IDYES)
			{ // 「はい」を選択したとき
				DestroyWindow(hWnd); //ウィンドウを破棄
			}
			break;
		}
		break;

	case WM_INPUT:
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		lpb.resize(dwSize);
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		{
			break;
		}

		raw = (RAWINPUT*)lpb.data();
		
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// 相対移動量
			g_cursorMove.x += raw->data.mouse.lLastX;
			g_cursorMove.y += raw->data.mouse.lLastY;
		}

		break;
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //既定の処理を返す
}


//=============================================================
// コンストラクタ
//=============================================================
Main::Main()
{
	// 分解能を設定
	timeBeginPeriod(1);
	m_currentTime = 0;
	m_execLastTime = timeGetTime();
	m_deltaTime = 0.0f;

	// FPS計測の初期化
	m_frameCount = 0;
	m_fpsLastTime = timeGetTime();
	m_fps = 0;
}

//=============================================================
// デストラクタ
//=============================================================
Main::~Main()
{
	// 分解能を戻す
	timeEndPeriod(1);

	// スレッドを破棄する
	if (m_thread != nullptr)
	{
		delete m_thread;
		m_thread = nullptr;
	}
}

//=============================================================
// 開始
//=============================================================
void Main::Start()
{
	m_startupTime = timeGetTime();
	Log::SendLog("メインループを開始しました");
}

//=============================================================
// メインループ
//=============================================================
void Main::MainLoop()
{
	// 現在時刻を取得
	m_currentTime = timeGetTime();

	// FPS値の計算
	if ((m_currentTime - m_fpsLastTime) >= 500)
	{ // 0.5秒経過毎
		m_fps = (m_frameCount * 1000) / (m_currentTime - m_fpsLastTime);
		m_fpsLastTime = m_currentTime;							// 計測した時刻を記録
		m_frameCount = 0;												// フレームカウントをクリア
	}

	// 更新
	if ((m_currentTime - m_execLastTime) >= (1000 / 60))
	{
		// デルタタイムを設定する
		m_deltaTime = (m_currentTime - m_execLastTime) * 0.001f;

		//処理開始時刻
		m_execLastTime = m_currentTime;

		// カーソルの移動量
		m_moveCursor = m_oldMoveCursor * 0.5f + g_cursorMove * 0.5f;

		// 更新処理
		Manager::GetInstance()->Update();

		// 描画処理
		Manager::GetInstance()->Draw();

		// フレームカウントを加算
		m_frameCount++;

		// リセット
		m_oldMoveCursor = g_cursorMove;
		g_cursorMove = { 0.0f, 0.0f };
	}
}

//=============================================================
// アクティブウィンドウか
//=============================================================
bool Main::IsActiveWindow()
{
	return g_isActiveWindow;
}

//=============================================================
// アプリケーション終了命令
//=============================================================
void Main::ExitApplication()
{
	g_isEnded = true;
}

//=============================================================
// カーソルの表示
//=============================================================
void Main::SetShowCursor(const bool& show)
{
	g_showCursor = show;
}

//=============================================================
// クライアント内のカーソルの位置を取得する
//=============================================================
Main::CursorPos Main::GetCursorClientPos()
{
	POINT points;
	GetCursorPos(&points);

	// スクリーン上で見た左上の座標を取得する
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(Manager::GetInstance()->GetRenderer()->GetHWND(), &startPos);

	CursorPos pos;
	pos.x = points.x - startPos.x;
	pos.y = points.y - startPos.y;

	// 画面サイズを考慮して変換する
	D3DXVECTOR2 rect = GetWindowSize();
	pos.x = static_cast<long>(pos.x * static_cast<float>(SCREEN_WIDTH / (float)rect.x));
	pos.y = static_cast<long>(pos.y * static_cast<float>(SCREEN_HEIGHT / (float)rect.y));
	return pos;
}

//=============================================================
// クライアント内のカーソルの位置を設定する
//=============================================================
void Main::SetCursorClientPos(long x, long y)
{
	CursorPos pos;
	pos.x = x;
	pos.y = y;

	// スクリーン上で見た左上の座標を取得する
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(Manager::GetInstance()->GetRenderer()->GetHWND(), &startPos);

	D3DXVECTOR2 rect = GetWindowSize();
	pos.x = static_cast<long>(pos.x * (rect.x / (float)SCREEN_WIDTH));
	pos.y = static_cast<long>(pos.y * (rect.y / (float)SCREEN_HEIGHT));

	pos.x += startPos.x;
	pos.y += startPos.y;

	SetCursorPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

//=============================================================
// カーソルの移動量の取得
//=============================================================
D3DXVECTOR2 Main::GetMoveCursor()
{
	return m_moveCursor;
}

//=============================================================
// ウィンドウサイズの取得
//=============================================================
D3DXVECTOR2 Main::GetWindowSize()
{
	RECT rect;
	GetClientRect(g_hwnd, &rect);
	return D3DXVECTOR2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
}

//=============================================================
// ウィンドウハンドルの取得
//=============================================================
HWND Main::GetHWND()
{
	return g_hwnd;
}
