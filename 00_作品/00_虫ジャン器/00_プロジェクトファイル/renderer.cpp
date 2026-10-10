//========================================================================================================
//
//レンダラー処理[renderer.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"

//========================================================================================================
//コンストラクタ
//========================================================================================================
Renderer::Renderer()
{
	//メンバ変数を初期化する
	this->m_pD3D = nullptr;   //Direct3D
	this->m_pD3DDevice = nullptr;   //Direct3Dデバイス
	this->m_pFont = nullptr;   //フォント
	this->m_isFullScreen = true;
	this->m_hWnd = nullptr;
}

//========================================================================================================
//デストラクタ
//========================================================================================================
Renderer::~Renderer()
{
}

//=======================================================================================================
//初期化処理
//=======================================================================================================
HRESULT Renderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;   //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;   //プレゼンテーションパラメーター

	//DirectX3Dオブジェクトの生成
	this->m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (this->m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得する
	if (FAILED(this->m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));   //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;   //ゲーム画面サイズ 幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;   //ゲーム画面サイズ 高さ
	d3dpp.BackBufferFormat = d3ddm.Format;   //バックバファの形式
	d3dpp.BackBufferCount = 1;   //バックバファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   //
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(this->m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &this->m_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア 頂点処理はCPUで行う)
		if (FAILED(this->m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->m_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(this->m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	this->m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	this->m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//サンプラーステートの設定
	this->m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	this->m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	this->m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	this->m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	this->m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	this->m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	this->m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//ウィンドウハンドルを保存する
	this->m_hWnd = hWnd;

	//デバッグ表示用フォントの生成
	D3DXCreateFont(this->m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &this->m_pFont);


	return S_OK;
}

//=======================================================================================================
//終了処理
//=======================================================================================================
void Renderer::Uninit()
{
	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	//デバッグ表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//=======================================================================================================
//更新処理
//=======================================================================================================
void Renderer::Update()
{
	//フルスクリーンの切り替え処理
	Keyboard* pKeyboard = Manager::Get()->GetKeyboard();
	if (pKeyboard->GetTrigger(DIK_F11))
	{//F11キーが押された場合
		//フルスクリーンを切り替える
		SetFullScreen(!this->m_isFullScreen);
	}
}

//=======================================================================================================
//描画処理
//=======================================================================================================
void Renderer::Draw()
{
	//画面クリア(バックバッファ Zバッファ ステンシルバッファのクリア)
	m_pD3DDevice->Clear(0, NULL, 
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ //描画開始が成功した場合
		//カメラが付いているオブジェクトを取得する
		std::vector<Camera*> vCamera = Component::Get<Camera>();

		//カメラが付いているオブジェクト分繰り返す
		for (unsigned int i = 0; i < vCamera.size(); i++)
		{
			//カメラを設定する
			vCamera[i]->Set();
		}

		//シーンの描画処理を行う
		Manager::Get()->GetScene()->Draw();

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=======================================================================================================
//フルスクリーン
//=======================================================================================================
void Renderer::SetFullScreen(bool bFullScreen)
{
	//フルスクリーンフラグを切り替える
	this->m_isFullScreen = bFullScreen;

	//現在のウィンドウスタイルを取得する
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//フルスクリーンフラグを確認する
	if (m_isFullScreen == true)
	{
		//ウィンドウモードに切り替える
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hWnd, SW_NORMAL);
	}
	else
	{
		//フルスクリーンモードに切り替える
		GetWindowRect(m_hWnd, &rect);
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}
}