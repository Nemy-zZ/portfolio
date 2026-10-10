//========================================================================================================
//
//桁の処理[digit.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "digit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Digit::Init()
{
	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//頂点バッファを生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,   //確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL);

	//更新処理を行う
	this->Update();
}

//========================================================================================================
//終了処理
//========================================================================================================
void Digit::Uninit()
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
void Digit::Update()
{
	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_2D* pVtx;   //頂点座標
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を設定する
	pVtx[0].pos = { this->m_pos.x - (this->m_size.x / 2), this->m_pos.y - (this->m_size.y / 2), 0.0f };
	pVtx[1].pos = { this->m_pos.x + (this->m_size.x / 2), this->m_pos.y - (this->m_size.y / 2), 0.0f };
	pVtx[2].pos = { this->m_pos.x - (this->m_size.x / 2), this->m_pos.y + (this->m_size.y / 2), 0.0f };
	pVtx[3].pos = { this->m_pos.x + (this->m_size.x / 2), this->m_pos.y + (this->m_size.y / 2), 0.0f };

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定する
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標を設定する
	pVtx[0].tex = { (1.0f / 10) * this->m_nValue, 0.0f };
	pVtx[1].tex = { (1.0f / 10) * (this->m_nValue + 1), 0.0f };
	pVtx[2].tex = { (1.0f / 10) * this->m_nValue, 1.0f };
	pVtx[3].tex = { (1.0f / 10) * (this->m_nValue + 1), 1.0f };

	//頂点バッファをアンロックする
	m_pVtxBuffer->Unlock();
}

//========================================================================================================
//描画処理
//========================================================================================================
void Digit::Draw(LPDIRECT3DTEXTURE9 pTexutre)
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//頂点バッファをデータストリームに設定する
	pDevice->SetStreamSource(0, this->m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャを設定する
	pDevice->SetTexture(0, pTexutre);

	//ポリゴンを描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}