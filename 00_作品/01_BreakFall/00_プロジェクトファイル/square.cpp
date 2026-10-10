//========================================================================================================
//
//四角形の処理[square.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "square.h"
#include "manager.h"
#include "object.h"
#include "transform.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Square::Init()
{
	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//頂点バッファを生成する
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,   //確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&this->m_pVtxBuffer,
		NULL);
}

//========================================================================================================
//終了処理
//========================================================================================================
void Square::Uninit()
{
	//頂点バッファを破棄する
	if (this->m_pVtxBuffer != nullptr)
	{
		this->m_pVtxBuffer->Release();
		this->m_pVtxBuffer = nullptr;
	}
}

//========================================================================================================
//更新処理
//========================================================================================================
void Square::Update()
{
	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_2D* pVtx;   //頂点座標
	this->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標を設定する
	D3DXVECTOR2 pos = this->GetObj()->GetComponent<Transform2D>()->GetPos();
	D3DXVECTOR2 size = this->GetObj()->GetComponent<Transform2D>()->GetSize();
	pVtx[0].pos = { pos.x - (size.x / 2), pos.y - (size.y / 2), 0.0f };
	pVtx[1].pos = { pos.x + (size.x / 2), pos.y - (size.y / 2), 0.0f };
	pVtx[2].pos = { pos.x - (size.x / 2), pos.y + (size.y / 2), 0.0f };
	pVtx[3].pos = { pos.x + (size.x / 2), pos.y + (size.y / 2), 0.0f };

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定する
	D3DXCOLOR col = this->GetColor();   //色
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ座標を設定する
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	//頂点バッファをアンロックする
	this->m_pVtxBuffer->Unlock();
}

//========================================================================================================
//描画処理
//========================================================================================================
void Square::Draw()
{
	//デバイスを取得する
	Renderer* pRenderer = Manager::Get()->GetRenderer();   //レンダラー
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();   //デバイスへのポインタ

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//頂点バッファをデータストリームに設定する
	pDevice->SetStreamSource(0, this->m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャを設定する
	pDevice->SetTexture(0, this->GetTexture());

	//ポリゴンを描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}