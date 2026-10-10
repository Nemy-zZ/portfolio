//------------------------------------------------------------
// @file		camera.cpp
// @brief	カメラ
// @detail	3D空間に存在するオブジェクトを描画するためのデバイス
//------------------------------------------------------------
#include "camera.h"
#include "system/manager.h"

std::vector<Camera*> Camera::m_cameras;
Camera* Camera::m_currentCamera = nullptr;

//=============================================================
// 初期化
//=============================================================
void Camera::Init()
{
	// 初期設定
	m_vecU = { 0.0f, 1.0f, 0.0f };
	m_posR = { 0.0f, 0.0f, 0.0f };
	m_length = CAMERA_LENGTH;
	m_fov = 45.0f;
	m_mode = MODE::MODE_PERSPECTIVE;
	m_addition = { 0.0f, 0.0f, 0.0f };
	m_clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 描画距離の設定
	m_clippingPlanes.min = CLIPPING_NEAR;
	m_clippingPlanes.max = CLIPPING_FAR;
	m_clippingPlanes.Sort();

	// ビューポートの設定
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	SetViewportPosition(0.0f, 0.0f);
	SetViewportSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	m_visible = true;

	// 描画ポリゴンの初期化
	InitRenderPolygon();

	// レンダーテクスチャ設定
	SetTextureSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// カメラリストに追加する
	m_cameras.push_back(this);
}

//=============================================================
// 終了
//=============================================================
void Camera::Uninit()
{
	// 描画ポリゴンの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}

	// レンダーテクスチャの破棄
	ReleaseRenderTextures();

	// ポストエフェクトの破棄
	for (auto fx : m_postEffects)
	{
		fx->Uninit();
		delete fx;
		fx = nullptr;
	}
	m_postEffects.clear();

	// カメラリストから削除する
	for (auto itr = m_cameras.begin(); itr != m_cameras.end(); itr++)
	{
		if (*itr == this)
		{
			m_cameras.erase(itr);
			break;
		}
	}
}

//=============================================================
// 更新
//=============================================================
void Camera::Update()
{
	// 前方から指定の長さ分伸ばす
	D3DXMATRIX rotMtx = transform->GetRotationMatrix();
	m_posR = { 0.0f, 0.0f, m_length };
	D3DXVec3TransformCoord(&m_posR, &m_posR, &rotMtx);

	// 位置を加算する
	m_posR += transform->GetWorldPosition();
	m_posR += m_addition;

	// ポストエフェクトを更新する
	for (auto fx : m_postEffects)
	{
		if (fx->IsActive())
		{
			fx->Update();
		}
	}
}

//=============================================================
// 描画ポリゴンの描画
//=============================================================
void Camera::DrawPolygon()
{
	if (!m_visible) return;

	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, m_renderTextures.color);

	// 現在のバックバッファを退避
	m_oldRT = nullptr;
	m_oldDS = nullptr;
	device->GetRenderTarget(0, &m_oldRT);
	device->GetDepthStencilSurface(&m_oldDS);
	
	// 最初の描画
	device->SetRenderTarget(0, m_surfPostA);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数

	// ポストエフェクトチェーン
	LPDIRECT3DTEXTURE9 inputTex = m_postA;
	LPDIRECT3DTEXTURE9 outputTex = m_postB;

	// サーフェイスを返すラムダ式
	auto GetSurface = [&](LPDIRECT3DTEXTURE9 tex) {
		if (tex == m_postA) return m_surfPostA;
		if (tex == m_postB) return m_surfPostB;
	};

	for (auto effect : m_postEffects)
	{
		// 非アクティブ
		if (!effect->IsActive())
		{
			continue;
		}

		// ポストエフェクトの設定
		effect->Set(this);

		while (effect->BeginTechnique())
		{
			// 出力先をセット
			device->SetRenderTarget(0, GetSurface(outputTex));

			// 処理中のテクスチャをセットする
			effect->SetProcessingTexture(inputTex);

			if (effect->Begin())
			{
				while (effect->BeginPass())
				{
					// ポリゴンの描画
					device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
						0, //描画する最初の頂点インデックス
						2); //描画するプリミティブ数

					effect->EndPass();
				}

				effect->End();

				// 入出力をスワップ
				std::swap(inputTex, outputTex);
			}

			// テクニックを終了する
			effect->EndTechnique();
		}
	}

	// 最終結果を格納する
	m_renderTextures.postProcessed = inputTex;

	// 元のバックバッファに戻す
	device->SetRenderTarget(0, m_oldRT);
	device->SetRenderTarget(1, nullptr);
	device->SetDepthStencilSurface(m_oldDS);

	// 破棄
	if (m_oldRT != nullptr)
	{
		m_oldRT->Release();
		m_oldRT = nullptr;
	}
	if (m_oldDS != nullptr)
	{
		m_oldDS->Release();
		m_oldDS = nullptr;
	}

	// 最終結果を描画する
	device->SetTexture(0, m_renderTextures.postProcessed);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数
}

//=============================================================
// テクスチャサイズを設定する
//=============================================================
bool Camera::SetTextureSize(const UINT& width, const UINT& height)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// レンダーテクスチャを破棄する
	ReleaseRenderTextures();

	// カラーテクスチャの作成
	if (FAILED(D3DXCreateTexture(
		device,
		width,
		height,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_renderTextures.color)))
	{
		return false;
	}
	m_renderTextures.color->GetSurfaceLevel(0, &m_colorSurface);

	// 深度テクスチャの作成
	if (FAILED(D3DXCreateTexture(
		device,
		width,
		height,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_L8,
		D3DPOOL_DEFAULT,
		&m_renderTextures.depth)))
	{
		return false;
	}
	m_renderTextures.depth->GetSurfaceLevel(0, &m_depthSurface);

	// ポストプロセス用Aテクスチャの作成
	if (FAILED(D3DXCreateTexture(
		device,
		width,
		height,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_postA)))
	{
		return false;
	}
	m_postA->GetSurfaceLevel(0, &m_surfPostA);

	// ポストプロセス用Bテクスチャの作成
	if (FAILED(D3DXCreateTexture(
		device,
		width,
		height,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_postB)))
	{
		return false;
	}
	m_postB->GetSurfaceLevel(0, &m_surfPostB);

	// 深度バッファの作成
	if (FAILED(device->CreateDepthStencilSurface(
		width,
		height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_depthStencil,
		NULL)))
	{
		return false;
	}

	// テクスチャサイズを記録する
	m_textureSize[0] = width;
	m_textureSize[1] = height;

	// 描画ポリゴンを更新する
	UpdateDrawPolygon();
}

//=============================================================
// 描画位置を設定する
//=============================================================
void Camera::SetDrawPosition(const D3DXVECTOR2& position)
{
	m_drawPosition = position;

	// 描画ポリゴンを更新する
	UpdateDrawPolygon();
}

//=============================================================
// ビューポートの位置を設定する
//=============================================================
void Camera::SetViewportPosition(float x, float y)
{
	m_viewport.X = static_cast<DWORD>(x);
	m_viewport.Y = static_cast<DWORD>(y);
}

//=============================================================
// ビューポートのサイズを設定する
//=============================================================
void Camera::SetViewportSize(float width, float height)
{
	m_viewport.Width = static_cast<DWORD>(width);
	m_viewport.Height = static_cast<DWORD>(height);
}

//=============================================================
// カメラの設置
//=============================================================
void Camera::SetCamera()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_projMtx);

	// ウィンドウサイズの取得
	D3DXVECTOR2 windowSize = {SCREEN_WIDTH, SCREEN_HEIGHT};

	// 投影モードごとに設定する
	switch (m_mode)
	{
	case Camera::MODE_PERSPECTIVE:
		D3DXMatrixPerspectiveFovLH(
			&m_projMtx,
			D3DXToRadian(m_fov),
			windowSize.x / windowSize.y,
			m_clippingPlanes.min,
			m_clippingPlanes.max);
		break;

	case Camera::MODE_PARALLEL:
		D3DXMatrixOrthoLH(
			&m_projMtx,
			windowSize.x,
			windowSize.y,
			m_clippingPlanes.min,
			m_clippingPlanes.max);

		// スケーリング調整
		D3DXMATRIX mtxScale;
		D3DXMatrixIdentity(&mtxScale);
		D3DXMatrixScaling(&mtxScale, 5.0f, 5.0f, 1.0f);
		D3DXMatrixMultiply(&m_projMtx, &m_projMtx, &mtxScale);
		break;
	}

	// プロジェクションマトリックスの設定
	device->SetTransform(D3DTS_PROJECTION, &m_projMtx);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_viewMtx);

	// ビューマトリックスの作成
	D3DXVECTOR3 pos = transform->GetWorldPosition();
	D3DXMatrixLookAtLH(&m_viewMtx, &pos, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	device->SetTransform(D3DTS_VIEW, &m_viewMtx);

	// ビューポートの設定
	device->SetViewport(&m_viewport);

	// 現在のカメラを設定する
	m_currentCamera = this;

	// 空
	Skybox* sky = gameObject->GetComponent<Skybox>();
	if (sky != nullptr)
	{
		sky->DrawSky();
	}
}

//=============================================================
// 描画開始
//=============================================================
bool Camera::Begin()
{
	// 設定チェック
	if (m_colorSurface == nullptr ||
		m_depthSurface == nullptr ||
		m_depthStencil == nullptr ||
		m_renderTextures.color == nullptr ||
		m_renderTextures.depth == nullptr)
	{
		return false;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 現在のバックバッファを退避
	m_oldRT = nullptr;
	m_oldDS = nullptr;
	device->GetRenderTarget(0, &m_oldRT);
	device->GetDepthStencilSurface(&m_oldDS);

	// MRTセット
	LPDIRECT3DSURFACE9 mrt[2] = { m_colorSurface, m_depthSurface };
	device->SetRenderTarget(0, mrt[0]);
	device->SetRenderTarget(1, mrt[1]);
	device->SetDepthStencilSurface(m_depthStencil);

	return true;
}

//=============================================================
// 描画終了
//=============================================================
void Camera::End()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 元のバックバッファに戻す
	device->SetRenderTarget(0, m_oldRT);
	device->SetRenderTarget(1, nullptr);
	device->SetDepthStencilSurface(m_oldDS);

	// 破棄
	if (m_oldRT != nullptr)
	{
		m_oldRT->Release();
		m_oldRT = nullptr;
	}
	if (m_oldDS != nullptr)
	{
		m_oldDS->Release();
		m_oldDS = nullptr;
	}
}

//=============================================================
// レンダーテクスチャの解放
//=============================================================
void Camera::ReleaseRenderTextures()
{
	// カラーテクスチャの破棄
	if (m_renderTextures.color != nullptr)
	{
		m_renderTextures.color->Release();
		m_renderTextures.color = nullptr;
	}

	// 深度テクスチャの破棄
	if (m_renderTextures.depth != nullptr)
	{
		m_renderTextures.depth->Release();
		m_renderTextures.depth = nullptr;
	}

	// ポストプロセス用Aテクスチャの破棄
	if (m_postA != nullptr)
	{
		m_postA->Release();
		m_postA = nullptr;
	}

	// ポストプロセス用Bテクスチャの破棄
	if (m_postB != nullptr)
	{
		m_postB->Release();
		m_postB = nullptr;
	}

	// カラーサーフェイスの破棄
	if (m_colorSurface != nullptr)
	{
		m_colorSurface->Release();
		m_colorSurface = nullptr;
	}

	// 深度サーフェイスの破棄
	if (m_depthSurface != nullptr)
	{
		m_depthSurface->Release();
		m_depthSurface = nullptr;
	}

	// 深度ステンシルサーフェイスの破棄
	if (m_depthStencil != nullptr)
	{
		m_depthStencil->Release();
		m_depthStencil = nullptr;
	}

	// 保持用のサーフェイスの破棄
	if (m_oldRT != nullptr)
	{
		m_oldRT->Release();
		m_oldRT = nullptr;
	}
	if (m_oldDS != nullptr)
	{
		m_oldDS->Release();
		m_oldDS = nullptr;
	}
}

//=============================================================
// 描画ポリゴンの初期化
//=============================================================
void Camera::InitRenderPolygon()
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	//頂点バッファの生成
	device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
	VERTEX_2D* vtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	//頂点座標の設定
	vtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;

	//頂点カラー
	vtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}

//=============================================================
// 描画ポリゴンの更新
//=============================================================
void Camera::UpdateDrawPolygon()
{
	VERTEX_2D* vtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	//頂点座標の設定
	vtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[1].pos = D3DXVECTOR3((float)m_textureSize[0], 0.0f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(0.0f, (float)m_textureSize[1], 0.0f);
	vtx[3].pos = D3DXVECTOR3((float)m_textureSize[0], (float)m_textureSize[1], 0.0f);

	D3DXVECTOR3 pos = { m_drawPosition.x, m_drawPosition.y, 0.0f };
	vtx[0].pos += pos;
	vtx[1].pos += pos;
	vtx[2].pos += pos;
	vtx[3].pos += pos;

	//頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}
