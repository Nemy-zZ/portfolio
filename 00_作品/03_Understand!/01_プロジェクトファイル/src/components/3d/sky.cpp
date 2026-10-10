//------------------------------------------------------------
// @file		sky.cpp
// @brief	空
//------------------------------------------------------------
#include "sky.h"
#include <fstream>

// 静的メンバ変数の初期化
constexpr float SKYBOX_SIZE = 10.0f;

//=============================================================
// 初期化
//=============================================================
void Skybox::Init()
{
	// 変数の初期化
	for (int i = 0; i < 6; i++)
	{
		m_vtxBuff[i] = nullptr;		// 頂点バッファ
		m_texture[i] = nullptr;		// テクスチャ
	}
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	for (int i = 0; i < 6; i++)
	{
		//頂点バッファの生成
		device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff[i], NULL);
		VERTEX_3D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_vtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		switch (i)
		{
		case TOP:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case BOTTOM:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		case FRONT:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case BACK:
			pVtx[0].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		case LEFT:
			pVtx[0].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(-SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			break;
		case RIGHT:
			pVtx[0].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[1].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			pVtx[2].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, SKYBOX_SIZE / 2);
			pVtx[3].pos = D3DXVECTOR3(SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2, -SKYBOX_SIZE / 2);
			break;
		}

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		m_vtxBuff[i]->Unlock();
	}

	// テクスチャタイプ
	SetTextureType(TEXTURETYPE::SINGLE);
}

//=============================================================
// 終了
//=============================================================
void Skybox::Uninit()
{
	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファの破棄
		if (m_vtxBuff[i] != nullptr)
		{
			m_vtxBuff[i]->Release();
			m_vtxBuff[i] = nullptr;
		}
	}
}

//=============================================================
// 空を描画する
//=============================================================
void Skybox::DrawSky()
{
	if (m_texture[0] == nullptr)
	{ // テクスチャが設定されていないときは無効
		return;
	}

	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = transform->GetTranslationMatrix();

	// ライトを無効にする
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点フォーマットの設定
	device->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファをデータストリームに設定
		device->SetStreamSource(0, m_vtxBuff[i], 0, sizeof(VERTEX_3D));

		// テクスチャの設定
		if (m_textureType == TEXTURETYPE::SINGLE)
		{
			device->SetTexture(0, m_texture[0]);
		}
		else
		{
			device->SetTexture(0, m_texture[i]);
		}

		// ポリゴンの描画
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0, //描画する最初の頂点インデックス
			2);				//描画するプリミティブ数
	}

	// ライトを有効に戻す
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================
// テクスチャタイプを設定する
//=============================================================
void Skybox::SetTextureType(TEXTURETYPE textureType)
{
	VERTEX_3D* pVtx; // 頂点情報へのポインタ
	m_textureType = textureType;

	for (int i = 0; i < 6; i++)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_vtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		switch (m_textureType)
		{
		case Skybox::SINGLE:
			switch (i)
			{
			case TOP:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				break;
			case BOTTOM:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f);
				break;
			case BACK:
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				break;
			case LEFT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 1, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				break;
			case FRONT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 2, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 2);
				break;
			case RIGHT:
				pVtx[0].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 1);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f / 3 * 1);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 4 * 3, 1.0f / 3 * 2);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f / 3 * 2);
				break;
			}
			break;
		case Skybox::SPLIT:
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}

		// 頂点バッファをアンロックする
		m_vtxBuff[i]->Unlock();
	}
}

//=============================================================
// テクスチャを設定する
//=============================================================
void Skybox::SetTexture(const std::string& path, const FACE& face)
{
	if (0 <= face && face < 6)
	{
		TextureData* texData = Manager::GetInstance()->GetResourceDataManager()->RefTexture(path);
		if (texData == nullptr) return;
		m_texture[face] = texData->GetTexture();
	}
}

//=============================================================
// スカイボックスファイルを読み込む
//=============================================================
void Skybox::LoadSkybox(const std::string& path)
{
	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());

	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		return;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jSkybox = json::parse(sInputData);

	// タイプを設定する
	if (jSkybox.contains("type"))
	{
		if (jSkybox["type"] == "single")
		{ // 1枚
			SetTextureType(TEXTURETYPE::SINGLE);
		}
		else if (jSkybox["type"] == "split")
		{ // 複数枚（6枚）
			SetTextureType(TEXTURETYPE::SPLIT);
		}
	}

	// テクスチャを変更する
	if (jSkybox.contains("texture"))
	{
		for (unsigned int i = 0; i < jSkybox["texture"].size(); i++)
		{
			SetTexture(jSkybox["texture"][i], (FACE)i);
		}
	}

	ifs.close();
}
