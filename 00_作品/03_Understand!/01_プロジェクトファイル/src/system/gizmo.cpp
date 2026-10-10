//------------------------------------------------------------
// @file		gizmo.cpp
// @brief	ギズモ
// @detail ギズモ機能を提供します
//------------------------------------------------------------
#include "gizmo.h"


//=============================================================
// 解放する
//=============================================================
void GizmoSystem::Release()
{
	// 2Dラインの終了
	for (auto itr = m_line2d.begin(); itr != m_line2d.end(); itr++)
	{
		(*itr).Get().Uninit();
	}
	m_line2d.clear();

	// 3Dラインの終了
	for (auto itr = m_line3d.begin(); itr != m_line3d.end(); itr++)
	{
		(*itr).Get().Uninit();
	}
	m_line3d.clear();
}

//=============================================================
// 3Dレンダリング
//=============================================================
void GizmoSystem::Render3D()
{
	// ライン3D
	for (auto itr = m_line3d.begin(); itr != m_line3d.end();)
	{
		// 描画
		if ((*itr).GetUse())
		{
			(*itr).Get().Draw();
		}

		// 更新
		if ((*itr).Update())
		{
			(*itr).Get().Uninit();
			itr = m_line3d.erase(itr);
			continue;
		}

		// 次へ
		(*itr).GetUse() = false;
		itr++;
	}
}

//=============================================================
// 2Dレンダリング
//=============================================================
void GizmoSystem::Render2D()
{
	// ライン2D
	for (auto itr = m_line2d.begin(); itr != m_line2d.end();)
	{
		// 描画
		if ((*itr).GetUse())
		{
			(*itr).Get().Draw();
		}

		// 更新
		if ((*itr).Update())
		{
			(*itr).Get().Uninit();
			itr = m_line2d.erase(itr);
			continue;
		}

		// 次へ
		(*itr).GetUse() = false;
		itr++;
	}
}

//=============================================================
// ラインの描画
//=============================================================
void GizmoSystem::DrawLine(const D3DXVECTOR3& from, const D3DXVECTOR3& to, const D3DXCOLOR& color)
{
	// 余っているとき
	for (auto itr = m_line3d.begin(); itr != m_line3d.end(); itr++)
	{
		if (!(*itr).GetUse())
		{
			(*itr).Get().Set(from, to, color);
			(*itr).GetUse() = true;
			return;
		}
	}

	// 新規作成
	GizmoObject<Line3D> data;
	data.Get().Init();
	data.Get().Set(from, to, color);
	m_line3d.emplace_back(data);
}

//=============================================================
// 2Dラインの描画
//=============================================================
void GizmoSystem::DrawLine2D(const D3DXVECTOR2& from, const D3DXVECTOR2& to, const float& bold, const D3DXCOLOR& color)
{
	// 余っているとき
	for (auto itr = m_line2d.begin(); itr != m_line2d.end(); itr++)
	{
		if (!(*itr).GetUse())
		{
			(*itr).Get().Set(from, to, bold, color);
			(*itr).GetUse() = true;
			return;
		}
	}

	// 新規作成
	GizmoObject<Line2D> data;
	data.Get().Init();
	data.Get().Set(from, to, bold, color);
	m_line2d.emplace_back(data);
}

//=============================================================
// 長方形の2Dラインの描画
//=============================================================
void GizmoSystem::DrawRectangle(const D3DXVECTOR2& start, const D3DXVECTOR2& size, const float& bold, const D3DXCOLOR& color, const float& angle)
{
	D3DXVECTOR2 point[4] =
	{
		{size.x * -0.5f, size.y * -0.5f },
		{size.x * 0.5f, size.y * -0.5f},
		{size.x * 0.5f, size.y * 0.5f},
		{size.x * -0.5f, size.y * 0.5f}
	};

	// 回転
	D3DXMATRIX mtx;
	D3DXMatrixRotationZ(&mtx, angle);
	for (int i = 0; i < 4; i++)
		D3DXVec2TransformCoord(&point[i], &point[i], &mtx);

	// 移動
	for (int i = 0; i < 4; i++)
		point[i] += start + size * 0.5f;

	// 線を描画する
	DrawLine2D(point[0], point[1], bold, color);
	DrawLine2D(point[1], point[2], bold, color);
	DrawLine2D(point[2], point[3], bold, color);
	DrawLine2D(point[3], point[0], bold, color);
}

//=============================================================
// ボックスの3Dラインの描画
//=============================================================
void GizmoSystem::DrawBox(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color, const D3DXMATRIX& matrix)
{
	D3DXVECTOR3 point[8] =
	{
		{start.x, start.y, start.z},
		{end.x, start.y, start.z},
		{end.x, start.y, end.z},
		{start.x, start.y, end.z},
		{start.x, end.y, start.z},
		{end.x, end.y, start.z},
		{end.x, end.y, end.z},
		{start.x, end.y, end.z}
	};

	// マトリックスをかける
	D3DXVec3TransformCoordArray(&point[0], sizeof(D3DXVECTOR3), &point[0], sizeof(D3DXVECTOR3), &matrix, 8);

	// 線を描画する
	DrawLine(point[0], point[1], color);
	DrawLine(point[1], point[2], color);
	DrawLine(point[2], point[3], color);
	DrawLine(point[3], point[0], color);

	DrawLine(point[0], point[4], color);
	DrawLine(point[1], point[5], color);
	DrawLine(point[2], point[6], color);
	DrawLine(point[3], point[7], color);

	DrawLine(point[4], point[5], color);
	DrawLine(point[5], point[6], color);
	DrawLine(point[6], point[7], color);
	DrawLine(point[7], point[4], color);
}


//=============================================================
// 3Dライン : 初期化
//=============================================================
void GizmoSystem::Line3D::Init()
{
	// バッファを確保する
	auto device = Manager::GetInstance()->GetRenderer()->GetDevice();
	device->CreateVertexBuffer(sizeof(VERTEX_3D) * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
}

//=============================================================
// 3Dライン : 終了
//=============================================================
void GizmoSystem::Line3D::Uninit()
{
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// 3Dライン : 描画
//=============================================================
void GizmoSystem::Line3D::Draw()
{
	auto device = Manager::GetInstance()->GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	device->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	device->SetTexture(0, nullptr);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_LINELIST, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		1);				//描画するプリミティブ数
}

//=============================================================
// 3Dライン : セット
//=============================================================
void GizmoSystem::Line3D::Set(const D3DXVECTOR3& from, const D3DXVECTOR3& to, const D3DXCOLOR& color)
{
	VERTEX_3D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// 頂点座標の設定
	vtx[0].pos = from;
	vtx[1].pos = to;

	// 法線ベクトルの設定
	vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	vtx[0].col = color;
	vtx[1].col = color;

	// テクスチャ座標の設定
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}


//=============================================================
// 2Dライン : 初期化
//=============================================================
void GizmoSystem::Line2D::Init()
{
	// バッファを確保する
	auto device = Manager::GetInstance()->GetRenderer()->GetDevice();
	device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
}

//=============================================================
// 2Dライン : 終了
//=============================================================
void GizmoSystem::Line2D::Uninit()
{
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// 2Dライン : 描画
//=============================================================
void GizmoSystem::Line2D::Draw()
{
	auto device = Manager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device->SetTexture(0, nullptr);

	// ポリゴンの描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数
}

//=============================================================
// 2Dライン : セット
//=============================================================
void GizmoSystem::Line2D::Set(const D3DXVECTOR2& from, const D3DXVECTOR2& to, const float& bold, const D3DXCOLOR& color)
{
	// 長さを計算する
	float length = sqrtf(fabsf(from.x - to.x) * fabsf(from.x - to.x) + fabsf(from.y - to.y) * fabsf(from.y - to.y));

	// fromからtoの角度を計算する
	float angle = atan2f(to.x - from.x, to.y - from.y) + D3DX_PI;

	// 4点の座標を計算する
	D3DXVECTOR2 points[4] =
	{
		{-bold * 0.5f, -length - bold * 0.5f},
		{bold * 0.5f, -length - bold * 0.5f},
		{-bold * 0.5f, bold * 0.5f},
		{bold * 0.5f, bold * 0.5f}
	};
	D3DXMATRIX rotMtx;
	D3DXMatrixIdentity(&rotMtx);
	D3DXMatrixRotationZ(&rotMtx, -angle);
	for (int i = 0; i < 4; i++)
		D3DXVec2TransformCoord(&points[i], &points[i], &rotMtx);

	// 移動させる
	for (int i = 0; i < 4; i++)
		points[i] += from;

	VERTEX_2D* vtx;
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	// 頂点座標の設定
	vtx[0].pos = { points[0].x, points[0].y, 0.0f };
	vtx[1].pos = { points[1].x, points[1].y, 0.0f };
	vtx[2].pos = { points[2].x, points[2].y, 0.0f };
	vtx[3].pos = { points[3].x, points[3].y, 0.0f };

	// rhw
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;

	// 頂点カラー
	vtx[0].col = color;
	vtx[1].col = color;
	vtx[2].col = color;
	vtx[3].col = color;

	// テクスチャ座標
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_vtxBuff->Unlock();
}
