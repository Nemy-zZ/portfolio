//========================================================================================================
//
//選択肢の処理[select_box.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "select_box.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数を初期化する
float SelectBox::INIT_FADE = 0.025f;

//========================================================================================================
//コンストラクタ
//========================================================================================================
SelectBox::SelectBox()
{
	//メンバ変数を初期化する
	this->m_pVtxBuffer = nullptr;   //バッファ
	this->m_nPattern = 0;
	this->m_nOder = 0;
	this->m_pos = { 0.0f, 0.0f };   //位置
	this->m_size = { 0.0f, 0.0f };   //サイズ
	this->m_color = { 1.0f, 1.0f, 1.0f, 0.5f };   //色
	this->m_isSelect = false;
	this->m_fFade = INIT_FADE;
}

//========================================================================================================
//デストラクタ
//========================================================================================================
SelectBox::~SelectBox()
{
}

//========================================================================================================
//初期化処理
//========================================================================================================
void SelectBox::Init()
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
}

//========================================================================================================
//終了処理
//========================================================================================================
void SelectBox::Uninit()
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
void SelectBox::Update()
{
	//選択フラグを確認する
	if (this->m_isSelect == true)
	{//選択フラグが立っている場合
		//透明度をフェードする
		this->m_color.a += this->m_fFade;
		if (this->m_color.a >= 1.0f || this->m_color.a <= 0.0f)
		{//最大値 or 最小値に達した場合
			//フェードを反転させる
			this->m_fFade *= -1;
		}
	}
	else
	{//立っていない場合
		//リセットする
		this->Reset();
	}

	//選択フラグを下ろす
	this->m_isSelect = false;

	//頂点座標をロックして、頂点情報へのポインタを取得する
	VERTEX_2D* pVtx;   //頂点座標
	this->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = this->m_color;
	pVtx[1].col = this->m_color;
	pVtx[2].col = this->m_color;
	pVtx[3].col = this->m_color;

	//テクスチャ座標を設定する
	pVtx[0].tex = { 0.0f, (1.0f / this->m_nPattern) * this->m_nOder };
	pVtx[1].tex = { 1.0f, (1.0f / this->m_nPattern) * this->m_nOder };
	pVtx[2].tex = { 0.0f, (1.0f / this->m_nPattern) * (this->m_nOder + 1) };
	pVtx[3].tex = { 1.0f, (1.0f / this->m_nPattern) * (this->m_nOder + 1) };

	//頂点バッファをアンロックする
	m_pVtxBuffer->Unlock();
}

//========================================================================================================
//描画処理
//========================================================================================================
void SelectBox::Draw(LPDIRECT3DTEXTURE9 pTexture)
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//頂点バッファをデータストリームに設定する
	pDevice->SetStreamSource(0, this->m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャを設定する
	pDevice->SetTexture(0, pTexture);

	//ポリゴンを描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================================================
//生成処理
//========================================================================================================
SelectBox* SelectBox::Create(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	//数字を生成する
	SelectBox* pSelectBox = new SelectBox;

	//情報を設定する
	pSelectBox->m_pos = pos;   //位置
	pSelectBox->m_size = size;   //サイズ

	//初期化する
	pSelectBox->Init();

	//数字を返す
	return pSelectBox;
}