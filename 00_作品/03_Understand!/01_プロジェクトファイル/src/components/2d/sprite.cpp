//------------------------------------------------------------
// @file		sprite.cpp
// @brief	スプライト
// @detail	2Dの板ポリゴン
//------------------------------------------------------------
#include "sprite.h"
#include "system/manager.h"

//=============================================================
// 初期化
//=============================================================
void Sprite::Init()
{
	m_isFirst = true;
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_monitor = new TransformMonitor(transform);

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
// 終了
//=============================================================
void Sprite::Uninit()
{
	//頂点バッファの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// 更新
//=============================================================
void Sprite::Update()
{
	if (m_isFirst || m_monitor->HasChanged())
	{
		// 頂点を更新する
		UpdateVertex();

		m_isFirst = false;
	}
}

//=============================================================
// UI描画
//=============================================================
void Sprite::DrawUI()
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, m_texture);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数
}

//=============================================================
// 色を設定する
//=============================================================
void Sprite::SetColor(const D3DXCOLOR& color)
{
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_2D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// 色を設定する
	vtx[0].col = color;
	vtx[1].col = color;
	vtx[2].col = color;
	vtx[3].col = color;

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();

	m_color = color;
}

//=============================================================
// テクスチャを設定する
//=============================================================
void Sprite::SetTexture(std::string path)
{
	// データを参照する
	TextureData* data = Manager::GetInstance()->GetResourceDataManager()->RefTexture(path);

	// テクスチャをバインドする
	if (data != nullptr)
	{
		BindTexture(data->GetTexture());
	}
}

//=============================================================
// UVを設定する
//=============================================================
void Sprite::SetUV(const D3DXVECTOR2& topLeft, const D3DXVECTOR2& topRight, const D3DXVECTOR2& bottomLeft, const D3DXVECTOR2& bottomRight)
{
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_2D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// UVを設定する
	vtx[0].tex = topLeft;
	vtx[1].tex = topRight;
	vtx[2].tex = bottomLeft;
	vtx[3].tex = bottomRight;

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}

//=============================================================
// 頂点を更新する
//=============================================================
void Sprite::UpdateVertex()
{
	VERTEX_2D* vtx; //頂点情報へのポインタ

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationZ(&mtx, transform->GetWorldEulerAngle().z);

	// 頂点位置を格納する
	D3DXVECTOR3 convPos[4] =
	{
		{0.0f, 0.0f, 0.0f},
		{transform->size.x, 0.0f, 0.0f},
		{0.0f, transform->size.y, 0.0f},
		{transform->size.x, transform->size.y, 0.0f}
	};

	// アンカーポイントを加味する
	for (int i = 0; i < 4; i++)
	{
		convPos[i].x -= m_anchorPoint.x;
		convPos[i].y -= m_anchorPoint.y;
	}


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// サイズと回転
	D3DXVec3TransformCoord(&vtx[0].pos, &convPos[0], &mtx);
	D3DXVec3TransformCoord(&vtx[1].pos, &convPos[1], &mtx);
	D3DXVec3TransformCoord(&vtx[2].pos, &convPos[2], &mtx);
	D3DXVec3TransformCoord(&vtx[3].pos, &convPos[3], &mtx);

	// 位置
	for (int i = 0; i < 4; i++)
	{
		vtx[i].pos.x += transform->GetWorldPosition().x;
		vtx[i].pos.y += transform->GetWorldPosition().y;
	}

	//頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}

//=============================================================
// 更新する
//=============================================================
void AnimationSprite::Update()
{
	// 更新
	Sprite::Update();

	// アニメーションの更新
	if (m_isPlaying)
	{
		// 経過時間
		m_elapsedTime += Main::GetInstance().GetDeltaTime();

		// 次へ
		if (m_elapsedTime >= m_frameTime)
		{
			m_idx++;

			// フレーム上限
			if (m_idx >= m_width * m_height)
			{
				if (m_isLoop)
				{
					m_idx = 0;
				}
				else
				{
					m_isPlaying = false;
				}
			}
		}
	}

	// 表示
	int width = m_idx % m_width;
	float heightPer = (m_idx / (float)m_width);
	int height = static_cast<int>(heightPer - fmodf(heightPer, 1.0f));

	float uvX = width / (float)m_width;
	float uvY = height / (float)m_height;
	float uvWidth = 1.0f / (float)m_width;
	float uvHeight = 1.0f / (float)m_height;

	SetUV(
		{ uvX, uvY },
		{ uvX + uvWidth, uvY },
		{ uvX, uvY + uvHeight },
		{ uvX + uvWidth, uvY + uvHeight }
	);
}

//=============================================================
// セルの区分けを設定する
//=============================================================
void AnimationSprite::SetCell(const int& width, const int& height)
{
	if (width > 0)
	{
		m_width = width;
	}
	if (height > 0)
	{
		m_height = height;
	}
}
