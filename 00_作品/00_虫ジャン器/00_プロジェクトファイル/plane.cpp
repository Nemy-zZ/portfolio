//========================================================================================================
//
//平面処理[plane.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "plane.h"
#include "manager.h"
#include "object.h"
#include "transform.h"

//静的メンバ変数を初期化する
const D3DXVECTOR2 Plane::BASE_SIZE = { 200.0f, 200.0f };   //基本サイズ

//========================================================================================================
//初期化処理
//========================================================================================================
void Plane::Init()
{
	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//頂点バッファを生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,   //確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL);
}

//========================================================================================================
//終了処理
//========================================================================================================
void Plane::Uninit()
{
	//頂点バッファを破棄する
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//========================================================================================================
//更新処理
//========================================================================================================
void PlaneXZ::Update()
{
	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_3D* pVtx;   //頂点座標
	this->GetVtxBuffer()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を設定する
	pVtx[0].pos = { -Plane::BASE_SIZE.x / 2, 0.0f, +Plane::BASE_SIZE.y / 2 };
	pVtx[1].pos = { +Plane::BASE_SIZE.x / 2, 0.0f, +Plane::BASE_SIZE.y / 2 };
	pVtx[2].pos = { -Plane::BASE_SIZE.x / 2, 0.0f, -Plane::BASE_SIZE.y / 2 };
	pVtx[3].pos = { +Plane::BASE_SIZE.x / 2, 0.0f, -Plane::BASE_SIZE.y / 2 };

	//法線ベクトルを設定する
	pVtx[0].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[1].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[2].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[3].nor = { 0.0f, 1.0f, 0.0f };

	//頂点カラーを設定する
	D3DXCOLOR color = GetColor();   //色
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//テクスチャ座標を設定する
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	//頂点バッファをアンロックする
	this->GetVtxBuffer()->Unlock();
}

//========================================================================================================
//更新処理
//========================================================================================================
void PlaneY::Update()
{
	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_3D* pVtx;   //頂点座標
	this->GetVtxBuffer()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を設定する
	pVtx[0].pos = { -Plane::BASE_SIZE.x / 2, +Plane::BASE_SIZE.y / 2, 0.0f };
	pVtx[1].pos = { +Plane::BASE_SIZE.x / 2, +Plane::BASE_SIZE.y / 2, 0.0f };
	pVtx[2].pos = { -Plane::BASE_SIZE.x / 2, -Plane::BASE_SIZE.y / 2, 0.0f };
	pVtx[3].pos = { +Plane::BASE_SIZE.x / 2, -Plane::BASE_SIZE.y / 2, 0.0f };

	//法線ベクトルを設定する
	pVtx[0].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[1].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[2].nor = { 0.0f, 1.0f, 0.0f };
	pVtx[3].nor = { 0.0f, 1.0f, 0.0f };

	//頂点カラーを設定する
	D3DXCOLOR color = GetColor();   //色
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//テクスチャ座標を設定する
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	//頂点バッファをアンロックする
	this->GetVtxBuffer()->Unlock();
}

//========================================================================================================
//描画処理
//========================================================================================================
void Plane::Draw()
{
	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//ワールドマトリックスを設定する
	D3DXMATRIX mtxWorld = this->GetObj()->GetComponent<Transform>()->GetMtxWorld();
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//頂点バッファをテータストリームに設定する
	pDevice->SetStreamSource(0, this->m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャを設定する
	pDevice->SetTexture(0, this->GetTexture());

	//描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================================================
//描画処理
//========================================================================================================
void Billboard::Draw()
{
	D3DXMATRIX mtxTrans;   //計算用マトリックス
	D3DXMATRIX mtxView, mtxViewInverse;   //ビューマトリックス取得用

	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zバッファの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (this->m_isMultiply)
	{//加算合成する場合
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//カメラの逆行列を求める
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&mtxViewInverse, NULL, &mtxView);

	//Y軸ビルボードか確認する
	if (this->m_isYAxis)
	{
		//Y軸回転情報の打ち消す
		mtxViewInverse._21 = 0.0f;
		mtxViewInverse._22 = 1.0f;
		mtxViewInverse._23 = 0.0f;
	}

	//移動情報の打ち消す
	mtxViewInverse._41 = 0.0f;
	mtxViewInverse._42 = 0.0f;
	mtxViewInverse._43 = 0.0f;

	//ワールドマトリックスを取得する
	D3DXMATRIX mtxWorld = this->GetObj()->GetComponent<Transform>()->GetMtxWorld();

	//逆行列を反映する
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxWorld);

	//ワールドマトリックスを設定する
	pDevice->SetTransform(D3DTS_WORLD, &mtxViewInverse);

	//頂点バッファをテータストリームに設定する
	pDevice->SetStreamSource(0, this->GetVtxBuffer(), 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャを設定する
	pDevice->SetTexture(0, this->GetTexture());   //設定

	//描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zバッファの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}