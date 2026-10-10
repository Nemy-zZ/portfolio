//------------------------------------------------------------
// @file		renderer.cpp
// @brief	レンダラー
// @detail	描画を行うシステムです
//------------------------------------------------------------
#include "renderer.h"
#include "gameobject.h"
#include "components/3d/camera.h"
#include "components/other/camera_options.h"
#include "components/3d/culling.h"

//=============================================================
// 初期化
//=============================================================
HRESULT Renderer::Init(HINSTANCE hInstance, HWND hWnd)
{
	D3DDISPLAYMODE d3ddm;					// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_d3d == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));														// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;												// 画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;											// 画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;												// バックバッファの形式
	d3dpp.BackBufferCount = 1;																	// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;									// ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;													// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;									// デプスバッファ
	d3dpp.Windowed = RENDERER_WINDOWED;											// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				// プレゼンテーションインターバル

		// Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &m_d3dDevice)))
	{
		// Direct3Dデバイスの生成（描画処理はハードウェアで頂点処理はCPUで行う）
		if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &m_d3dDevice)))
		{
			// Direct3Dデバイスの生成（描画処理と頂点処理をCPUで行う）
			if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &m_d3dDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// ハンドルの代入
	m_hwnd = hWnd;

	// レンダーステートの設定
	m_d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 成功を返す
	return S_OK;
}

//=============================================================
// 終了
//=============================================================
void Renderer::Uninit()
{
	// Direct3Dデバイスの破棄
	if (m_d3dDevice != nullptr)
	{
		m_d3dDevice->Release();
		m_d3dDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_d3d != nullptr)
	{
		m_d3d->Release();
		m_d3d = nullptr;
	}

	// ギズモの終了
	Gizmo.Release();
}

//=============================================================
// 更新
//=============================================================
void Renderer::Update()
{
	if (AppInput->GetKeyDown(KeyCode::F11))
	{
		SetFullScreen(!GetFullScreen());
	}
}

//=============================================================
// 描画
//=============================================================
void Renderer::Draw()
{
	// カメラを取得する
	auto cameras = Component::GetComponents<Camera>();

	// 画面クリア（バッファクリア＆Zバッファクリア）
	m_d3dDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	if (SUCCEEDED(m_d3dDevice->BeginScene()))
	{ // 描画開始が成功した場合

		for (auto itr = cameras.begin(); itr != cameras.end(); itr++)
		{
			// 非アクティブのとき
			if (!((*itr)->gameObject->GetActive() && (*itr)->GetActive()))
			{
				continue;
			}

			if ((*itr)->Begin())
			{
				// 画面クリア（バッファクリア＆Zバッファクリア）
				m_d3dDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), (*itr)->GetClearColor(), 1.0f, 0);

				// カメラをセットする
				(*itr)->SetCamera();

				// カリングの判定処理
				CullingManager::GetInstance().Update(*itr);

				// シェーダー描画
				for (int renderLayer = 0; renderLayer < Shader::RenderLayer::MAX; renderLayer++)
				{
					// レンダーレイヤを設定する
					Shader::SetRenderLayer((Shader::RenderLayer)renderLayer);

					// ゲームオブジェクトを描画する
					LimitObjects* limitObjects = (*itr)->gameObject->GetComponent<LimitObjects>();
					if (limitObjects != nullptr)
					{
						limitObjects->LimitDraw();
					}
					else
					{
						GameObject::AllDraw();
					}
				}

				// ギズモの3D描画
				Gizmo.Render3D();

				// カリングの描画
				CullingManager::GetInstance().Draw(*itr);

				// 描画終了
				(*itr)->End();

				// ポリゴン描画
				(*itr)->DrawPolygon();
			}
		}

		// 画面全体のビューポートを設定する
		D3DVIEWPORT9 viewport;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		m_d3dDevice->SetViewport(&viewport);

		// ゲームオブジェクトをUI描画する
		GameObject::AllDrawUI();

		// ギズモの2D描画
		Gizmo.Render2D();

		// 描画終了
		m_d3dDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

	// カリングの結果を取得する
	CullingManager::GetInstance().Result();
}

//=============================================================
// フルスクリーン切り替え
//=============================================================
void Renderer::SetFullScreen(const bool& enabled)
{
	// 変更する
	m_isFullScreen = enabled;

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // ウィンドウの座標を格納

	// フルスクリーンに切り替える
	if (m_isFullScreen)
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(m_hwnd, &rect);
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}
	else
	{
		// ウィンドウモードに切り替え
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_NORMAL);
	}
}
