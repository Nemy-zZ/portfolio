//------------------------------------------------------------
// @file		culling.cpp
// @brief	カリング
//------------------------------------------------------------
#include "culling.h"
#include "components/other/camera_options.h"
#include "components/3d/model.h"
#include "components/3d/lod.h"

//=============================================================
// 初期化
//=============================================================
void CullingData::Init()
{
}

//=============================================================
// 終了
//=============================================================
void CullingData::Uninit()
{
}

//=============================================================
// カリング描画
//=============================================================
void CullingData::DrawCulling(OcclusionCulling* occlusion)
{
	if (m_isOcclusion)
	{
		auto device = Manager::GetInstance()->GetDevice();
		LOD* lod = gameObject->GetComponent<LOD>();
		ModelData* model = gameObject->GetComponent<Model>()->GetModelData();
		if (lod != nullptr)
		{
			model = lod->GetHighLODModel();
		}
		
		if (model == nullptr) return;

		// クエリを始める
		auto query = occlusion->GetQuery(gameObject);
		if (query == nullptr) return;
		if (FAILED(query->Issue(D3DISSUE_BEGIN)))
		{
			Log::SendLog("クエリの開始に失敗");
			return;
		}

		// 不透明でZバッファのみの簡易描画（バウンディングボックス推奨）
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// ワールドマトリックスの設定
		device->SetTransform(D3DTS_WORLD, &transform->GetMatrix());

		// 描画
		for (int i = 0; i < static_cast<int>(model->GetNumMaterial()); i++)
		{
			// モデル（パーツ）の描画
			model->GetMesh()->DrawSubset(i);
		}

		// クエリを終了する
		query->Issue(D3DISSUE_END);

		// 戻す
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}
}

//=============================================================
// 半径を取得する
//=============================================================
const float CullingData::GetRadius()
{
	if (m_radius > 0.0f) return m_radius;

	Model* model = gameObject->GetComponent<Model>();
	if (model != nullptr)
	{
		ModelData* data = model->GetModelData();
		if (data != nullptr)
		{
			return data->GetMaxRadius(transform->GetWorldScale());
		}
	}

	// 未割り当て
	return 1.0f;
}

//=============================================================
// カリング情報をリセットする
//=============================================================
void CullingData::ResetCulledData()
{
	m_culledFrustum = false;
	m_culledOcclusion = false;
}


//=============================================================
// カリング情報の更新
//=============================================================
void CullingManager::Update(Camera* camera)
{
	GameObject* cameraObj = camera->gameObject;

	// カリング情報を取得する
	std::vector<CullingData*> cullingDatas;
	for (GameObject* obj : GameObject::GetAllGameObjects())
	{
		CullingData* data = obj->GetComponent<CullingData>();
		if (data != nullptr)
		{
			data->ResetCulledData();
			data->SetDraw(true);
			cullingDatas.push_back(data);
		}
	}

	// カリング数情報をリセットする
	m_frustumCullNum = 0;
	m_occlusionCullNum = 0;

	// フラスタムカリング
	FrustumCulling* frustum = cameraObj->GetComponent<FrustumCulling>();
	if (frustum != nullptr)
	{
		// フラスタムを計算する
		frustum->UpdateFrustum();

		for (auto data : cullingDatas)
		{
			// カリングするか
			if (data->IsFrustumCulling() && !frustum->IsDraw(data->transform->GetWorldPosition(), data->GetRadius()))
			{
				data->SetCulledFrustum(true);
				data->SetDraw(false);
				m_frustumCullNum++;
			}
		}
	}

	// オクルージョンカリング
	OcclusionCulling* occlusion = cameraObj->GetComponent<OcclusionCulling>();
	if (occlusion != nullptr)
	{
		for (auto data : cullingDatas)
		{
			// 表示しない
			if (!data->IsDraw()) continue;

			// カリングするか
			if (data->IsOcclusionCulling())
			{
				// データをセットする
				occlusion->SetData(data->gameObject);

				// 前回のデータを参照し、描画するかを決定する
				if (!occlusion->IsDraw(data->gameObject))
				{
					data->SetCulledOcclusion(true);
					data->SetDraw(false);
					m_occlusionCullNum++;
				}
			}
		}

		// データをリセットする
		occlusion->ResetData();
	}
}

//=============================================================
// カリング描画
//=============================================================
void CullingManager::Draw(Camera* camera)
{
	OcclusionCulling* occlusion = camera->gameObject->GetComponent<OcclusionCulling>();
	if (occlusion != nullptr)
	{
		// 画面クリア（バッファクリア＆Zバッファクリア）
		Manager::GetInstance()->GetDevice()->Clear(0, nullptr, (D3DCLEAR_ZBUFFER), camera->GetClearColor(), 1.0f, 0);

		// カリング情報を取得する
		for (GameObject* obj : GameObject::GetAllGameObjects())
		{
			CullingData* data = obj->GetComponent<CullingData>();
			if (data != nullptr)
			{
				// フラスタムカリングされていないオブジェクトのみ
				if (data->IsOcclusionCulling() && !data->IsCulledFrustum())
				{
					// 描画するオブジェクトが制限されている場合は考慮する
					LimitObjects* limitObjects = camera->gameObject->GetComponent<LimitObjects>();
					if (limitObjects != nullptr)
					{
						if (limitObjects->IsDrawObject(data->gameObject))
						{
							data->DrawCulling(occlusion);
						}
					}
					else
					{
						data->DrawCulling(occlusion);
					}
				}
			}
		}
	}
}

//=============================================================
// 結果
//=============================================================
void CullingManager::Result()
{
	auto occlusions = Component::GetComponents<OcclusionCulling>();
	for (auto occlusion : occlusions)
	{
		occlusion->Result();
	}
}
