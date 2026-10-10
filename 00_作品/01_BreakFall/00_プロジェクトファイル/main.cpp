//********************************************************************************************************
//
//メイン処理[main.cpp]
// Author Murata Shinnosuke
//
//********************************************************************************************************
#include "main.h"
#include "manager.h"

// メモリーリーク検出
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define FIND_MEM_LEAKS
#endif // _DEBUG

#ifdef FIND_MEM_LEAKS
#define _NEW new (NORMALBLOCK, FILE, __LINE)
#else
#define _NEW new
#endif // FIND_MEM_LEAKS

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//グローバル変数を宣言
int g_nCountFPS = 0;   //fpsカウンタ

//=======================================================================================================
//メイン関数
//=======================================================================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// メモリーリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(386);
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION),
	};

	HWND hWnd; //ウインドウハンドル
	MSG msg; //メッセージを格納する変数

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;   //フレームカウント
	DWORD dwFPSLastTime;   //最後にFPSを計測して時刻

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	//マネージャーを生成する
	Manager::Create();
	Manager* pManager = Manager::Get();
	pManager->Init(hInstance, hWnd, TRUE);

	//分解能の設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);   //ウインドウ表示状態設定
	UpdateWindow(hWnd);   //クライアント領域を更新

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{ //WM_QUITメッセージを受け取った場合
				//ループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ //DirectXの処理
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			//更新処理
			pManager->Update();

			//描画処理
			pManager->Draw();
		}
	};

	//終了処理
	pManager->Uninit();
	delete pManager;
	pManager = nullptr;

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=======================================================================================================
//ウインドウプロシージャ
//=======================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//変数を宣言
	int nID;   //返り値格納

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_CLOSE:   //閉じるボタン
		//メッセージBOXを表示する
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{//yesの場合
			////メッセージBOX表示する
			//MessageBox(hWnd, "おつあくあー！", "湊あくあ", MB_OK | MB_ICONINFORMATION);

			//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;   //0を返す
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:   //escapeキー
			//メッセージBOXを表示する
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{ //yesの場合
				//メッセージBOX表示する
				//MessageBox(hWnd, "おつあくあー！", "湊あくあ", MB_OK | MB_ICONINFORMATION);

				//ウィンドウを破棄する
				DestroyWindow(hWnd);
			}
			break;

		case VK_SPACE:   //spaceキー
			//メッセージBOX表示する
			//MessageBox(hWnd, "あくたん！あくたん！", "湊あくあ", MB_OK | MB_ICONINFORMATION);
			break;

		case 0x41:   //aキー
			//メッセージBOX表示する
			//MessageBox(hWnd, "あてぃしのこと好きすぎ～", "湊あくあ", MB_OK | MB_ICONINFORMATION);
			break;

		case 0x34:   //4キー
			//メッセージBOX表示する
			//MessageBox(hWnd, "44.5...", "湊あくあ", MB_OK | MB_ICONINFORMATION);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}