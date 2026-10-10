//------------------------------------------------------------
// @file		billboard.cpp
// @brief	ビルボード
//------------------------------------------------------------
#include "billboard.h"

//=============================================================
// 初期化
//=============================================================
void Billboard::Init()
{
	// 変数の初期化
	m_vtxBuff = nullptr;											// 頂点バッファ
	m_texture = nullptr;											// テクスチャ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー
	m_monitor = new TransformMonitor(transform);

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 頂点バッファの生成
	device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff, NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// 頂点座標の設定
	vtx[0].pos = D3DXVECTOR3(-transform->size.x * 0.5f, transform->size.y * 0.5f, 0.0f);
	vtx[1].pos = D3DXVECTOR3(transform->size.x * 0.5f, transform->size.y * 0.5f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(-transform->size.x * 0.5f, -transform->size.y * 0.5f, 0.0f);
	vtx[3].pos = D3DXVECTOR3(transform->size.x * 0.5f, -transform->size.y * 0.5f, 0.0f);

	// 法線の設定
	vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	vtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	vtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}

//=============================================================
// 終了
//=============================================================
void Billboard::Uninit()
{
	// 頂点バッファの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// 更新
//=============================================================
void Billboard::Update()
{
	if (m_monitor->HasSizeChanged())
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		VERTEX_3D* vtx;
		m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

		// 頂点座標の設定
		vtx[0].pos = D3DXVECTOR3(-transform->size.x * 0.5f, transform->size.y * 0.5f, 0.0f);
		vtx[1].pos = D3DXVECTOR3(transform->size.x * 0.5f, transform->size.y * 0.5f, 0.0f);
		vtx[2].pos = D3DXVECTOR3(-transform->size.x * 0.5f, -transform->size.y * 0.5f, 0.0f);
		vtx[3].pos = D3DXVECTOR3(transform->size.x * 0.5f, -transform->size.y * 0.5f, 0.0f);

		// 法線の設定
		vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		vtx[0].col = m_color;
		vtx[1].col = m_color;
		vtx[2].col = m_color;
		vtx[3].col = m_color;

		// テクスチャ座標の設定
		vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		m_vtxBuff->Unlock();
	}
}

//=============================================================
// 描画
//=============================================================
void Billboard::Draw()
{
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx;
	D3DXMATRIX mtxView;

	// ライトを無効にする
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHAREF, 0);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ビューマトリックスを取得
	device->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&mtx, NULL, &mtxView);	// 逆行列を求める
	mtx._41 = 0.0f;
	mtx._42 = 0.0f;
	mtx._43 = 0.0f;

	D3DXMATRIX rotMtx;
	D3DXMatrixRotationZ(&rotMtx, transform->GetWorldEulerAngle().z);
	mtx *= rotMtx;

	// オブジェクトのマトリックスを掛ける
	D3DXMATRIX mtxTrans = transform->GetTranslationMatrix();
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device->SetTexture(0, m_texture);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2);				//描画するプリミティブ数

	// ライトを有効に戻す
	device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効に戻す
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================
// テクスチャの設定
//=============================================================
void Billboard::SetTexture(const std::string& path)
{
	BindTexture(Manager::GetInstance()->GetResourceDataManager()->RefTexture(path)->GetTexture());
}

//=============================================================
// 色
//=============================================================
void Billboard::SetColor(const D3DXCOLOR& color)
{
	m_color = color;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// 頂点カラー
	vtx[0].col = m_color;
	vtx[1].col = m_color;
	vtx[2].col = m_color;
	vtx[3].col = m_color;

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}