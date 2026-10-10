//==========================================================================================================
//
//線の処理[line.cpp]
// Author Murata Shinnosuke
//
//==========================================================================================================
#include "line.h"
#include "manager.h"

//==========================================================================================================
//初期化処理
//==========================================================================================================
void Line::Init()
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();

	//頂点バッファを生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&this->m_pVtxBuffer, 
		nullptr);
}

//==========================================================================================================
//終了処理
//==========================================================================================================
void Line::Uninit()
{
	//頂点バッファを破棄する
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//==========================================================================================================
//更新処理
//==========================================================================================================
void Line::Update()
{
	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_3D* pVtx;   //頂点座標
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を設定する
	pVtx[0].pos = this->m_posStart;
	pVtx[1].pos = this->m_posEnd;

	//法線ベクトルを設定する
	pVtx[0].nor = { 0.0f, 0.0f, -1.0f };
	pVtx[1].nor = { 0.0f, 0.0f, -1.0f };

	//頂点カラーを設定する
	pVtx[0].col = this->m_color;
	pVtx[1].col = this->m_color;

	//テクスチャ座標を設定する
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 0.0f, 0.0f };

	//頂点バッファをアンロックする
	m_pVtxBuffer->Unlock();
}

//==========================================================================================================
//描画処理
//==========================================================================================================
void Line::Draw()
{
	D3DXMATRIX mtxRot, mtxTrans;   //計算用マトリックス

	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//ワールドマトリックスを設定する
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをテータストリームに設定する
	pDevice->SetStreamSource(0, this->m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャを設定する
	pDevice->SetTexture(0, nullptr);

	//描画する
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}