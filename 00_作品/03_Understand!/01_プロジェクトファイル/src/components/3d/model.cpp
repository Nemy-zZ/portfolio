//------------------------------------------------------------
// @file		model.cpp
// @brief	モデル
// @detail	3Dのモデル
//------------------------------------------------------------
#include "model.h"
#include "system/manager.h"
#include "camera.h"
#include "components/other/camera_options.h"
#include "components/3d/culling.h"
#include "components/3d/lod.h"
#include "components/other/shader.h"
#include <filesystem>

//=============================================================
// 初期化
//=============================================================
void Model::Init()
{
	// シェーダー設定
	if (gameObject->GetComponent<Shader>() == nullptr)
	{
		gameObject->AddComponent<Shader>();
		gameObject->GetComponent<Shader>()->AddShader("");
	}

	m_colorBlack = false;
	m_colorEnabled = false;
}

//=============================================================
// 終了
//=============================================================
void Model::Uninit()
{
}

//=============================================================
// 描画
//=============================================================
void Model::Draw()
{
	// シェーダーの取得
	Shader* shader = gameObject->GetComponent<Shader>();
	if (shader != nullptr && shader->IsDrawCall())
	{
		LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();
		D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
		D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ
		ModelData* modelData = m_modelData;

		// LOD
		LOD* lod = gameObject->GetComponent<LOD>();
		if (lod != nullptr)
		{
			modelData = lod->GetLODModel();
		}

		// モデルデータ無し
		if (modelData == nullptr)
			return;

		// ワールドマトリックスの設定
		device->SetTransform(D3DTS_WORLD, &transform->GetMatrix());

		// 現在のマテリアルを取得
		device->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelData->GetBufferMaterial()->GetBufferPointer();

		// カラー変更
		if (m_colorEnabled)
		{
			for (int i = 0; i < static_cast<int>(modelData->GetNumMaterial()); i++)
			{
				pMat[i].MatD3D.Diffuse = m_colorBlack ? D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		// セット
		shader->Set();

		while (shader->Begin())
		{
			while (shader->BeginPass())
			{
				for (int i = 0; i < static_cast<int>(modelData->GetNumMaterial()); i++)
				{
					// マテリアルの設定
					device->SetMaterial(&pMat[i].MatD3D);

					// テクスチャの設定
					device->SetTexture(0, pMat[i].pTextureFilename != nullptr ? m_textures[i] : nullptr);

					// シェーダーにマテリアル情報を渡す
					shader->SetMaterial(pMat[i].MatD3D, m_textures[i]);

					// モデル（パーツ）の描画
					modelData->GetMesh()->DrawSubset(i);
				}
				shader->EndPass();
			}
			shader->End();
		}

		// 保存していたマテリアルに戻す
		device->SetMaterial(&matDef);
	}
}

//=============================================================
// 色設定
//=============================================================
void Model::SetColorBlack(const bool& enabled)
{
	m_colorBlack = enabled;
}

//=============================================================
// 色設定有効
//=============================================================
void Model::SetColorEnabled(const bool& enabled)
{
	m_colorEnabled = enabled;
}

//=============================================================
// 描画
//=============================================================
void Model::Load(std::string path)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// メッシュ情報を破棄
	Uninit();

	// メッシュを読み込む
	m_modelData = Manager::GetInstance()->GetResourceDataManager()->RefModel(path);

	// モデルテクスチャの読み込み
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_modelData->GetBufferMaterial()->GetBufferPointer();
	m_textures.resize(m_modelData->GetNumMaterial());
	for (int nCntMat = 0; nCntMat < (int)m_modelData->GetNumMaterial(); nCntMat++)
	{
		if (mat[nCntMat].pTextureFilename != nullptr)
		{ // テクスチャがあるとき

			// テクスチャの存在チェック
			if (!PathFileExistsA(mat[nCntMat].pTextureFilename))
			{ // 存在しないとき
				mat[nCntMat].pTextureFilename = nullptr;
				m_textures[nCntMat] = nullptr;
				continue;
			}

			// テクスチャを作成
			m_textures[nCntMat] = Manager::GetInstance()->GetResourceDataManager()->RefTexture(mat[nCntMat].pTextureFilename)->GetTexture();
		}
		else
		{
			m_textures[nCntMat] = nullptr;
		}
	}
}
