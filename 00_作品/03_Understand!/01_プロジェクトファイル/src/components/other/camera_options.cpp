//------------------------------------------------------------
// @file			camera_options.cpp
// @brief		カメラのオプション
//------------------------------------------------------------
#include "camera_options.h"
#include "components/3d/camera.h"
#include "components/3d/culling.h"

//=============================================================
// オブジェクトを追加する
//=============================================================
void LimitObjects::AddObject(GameObject* object)
{
	if (std::find(m_objects.begin(), m_objects.end(), object) == m_objects.end())
	{
		m_objects.push_back(object);
	}

	auto& gameObjects = GameObject::GetAllGameObjects();
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
	{
		if ((*itr)->transform->GetParent() == object->transform)
		{
			AddObject((*itr));
		}
	}
}

//=============================================================
// オブジェクトを削除する
//=============================================================
void LimitObjects::RemoveObject(GameObject* object)
{
	auto itr = std::find(m_objects.begin(), m_objects.end(), object);
	if (itr != m_objects.end())
	{
		m_objects.erase(itr);
	}
}

//=============================================================
// オブジェクトをリセットする
//=============================================================
void LimitObjects::ResetObject()
{
	m_objects.clear();
}

//=============================================================
// 描画する
//=============================================================
void LimitObjects::LimitDraw()
{
	if (m_type == WHITE)
	{
		for (auto obj : m_objects)
		{
			// 非アクティブ時はスルー
			if (!obj->GetActive() && !m_ignoreActive)
			{
				continue;
			}

			// カリング
			CullingData* cull = obj->GetComponent<CullingData>();
			if (cull != nullptr)
			{
				if (!cull->IsDraw())
				{
					continue;
				}
			}

			Shader* shader = obj->GetComponent<Shader>();
			if (shader != nullptr || (shader == nullptr && Shader::GetRenderLayer() == Shader::RenderLayer::Opaque))
			{
				obj->Draw();
			}
		}
	}
	else if (m_type == BLACK)
	{
		auto& gameObjects = GameObject::GetAllGameObjects();
		for (auto itr = gameObjects.begin(); itr != gameObjects.end(); itr++)
		{
			bool isExist = std::find(m_objects.begin(), m_objects.end(), *itr) != m_objects.end();
			if (isExist) continue;

			// 非アクティブ時はスルー
			if (!(*itr)->GetActive() && !m_ignoreActive)
			{
				continue;
			}

			// カリング
			CullingData* cull = (*itr)->GetComponent<CullingData>();
			if (cull != nullptr)
			{
				if (!cull->IsDraw())
				{
					continue;
				}
			}

			// 描画
			(*itr)->Draw();
		}
	}
}

//=============================================================
// 描画するオブジェクトかを判定する
//=============================================================
bool LimitObjects::IsDrawObject(GameObject* object)
{
	if (m_type == WHITE)
	{
		// 非アクティブ時はスルー
		if (!object->GetActive() && !m_ignoreActive)
		{
			return false;
		}

		Shader* shader = object->GetComponent<Shader>();
		if (shader != nullptr || (shader == nullptr && Shader::GetRenderLayer() == Shader::RenderLayer::Opaque))
		{
			return true;
		}
	}
	else if (m_type == BLACK)
	{
		bool isExist = std::find(m_objects.begin(), m_objects.end(), object) != m_objects.end();
		if (isExist) return false;

		// 非アクティブ時はスルー
		if (!object->GetActive() && !m_ignoreActive)
		{
			return false;
		}

		return true;
	}

	return false;
}


//=============================================================
// 更新
//=============================================================
void FrustumCulling::UpdateFrustum()
{
	Camera* camera = gameObject->GetComponent<Camera>();
	if (camera == nullptr) return;

	D3DXMATRIX matViewProj, matView, matProj;
	matView = camera->GetViewMatrix();
	matProj = camera->GetProjectionMatrix();
	D3DXMatrixIdentity(&matViewProj);
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);

	// 視錐台の平面を抽出
	m_planes[0] = D3DXPLANE(matViewProj._14 + matViewProj._11, matViewProj._24 + matViewProj._21, matViewProj._34 + matViewProj._31, matViewProj._44 + matViewProj._41); // 左
	m_planes[1] = D3DXPLANE(matViewProj._14 - matViewProj._11, matViewProj._24 - matViewProj._21, matViewProj._34 - matViewProj._31, matViewProj._44 - matViewProj._41); // 右
	m_planes[2] = D3DXPLANE(matViewProj._14 - matViewProj._12, matViewProj._24 - matViewProj._22, matViewProj._34 - matViewProj._32, matViewProj._44 - matViewProj._42); // 上
	m_planes[3] = D3DXPLANE(matViewProj._14 + matViewProj._12, matViewProj._24 + matViewProj._22, matViewProj._34 + matViewProj._32, matViewProj._44 + matViewProj._42); // 下
	m_planes[4] = D3DXPLANE(matViewProj._13, matViewProj._23, matViewProj._33, matViewProj._43); // 近
	m_planes[5] = D3DXPLANE(matViewProj._14 - matViewProj._13, matViewProj._24 - matViewProj._23, matViewProj._34 - matViewProj._33, matViewProj._44 - matViewProj._43); // 遠

	// 正規化
	for (int i = 0; i < 6; i++)
	{
		float len = sqrtf(
			m_planes[i].a * m_planes[i].a +
			m_planes[i].b * m_planes[i].b +
			m_planes[i].c * m_planes[i].c
		);

		if (len != 0.0f)
		{
			m_planes[i].a /= len;
			m_planes[i].b /= len;
			m_planes[i].c /= len;
			m_planes[i].d /= len;
		}
	}

	// カリング数をリセット
	m_cullingNum = 0;
	m_drawNum = 0;
}

//=============================================================
// 表示するか
//=============================================================
bool FrustumCulling::IsDraw(const D3DXVECTOR3& center, const float& radius)
{
	// 判定する
	for (int i = 0; i < 6; i++)
	{
		float distance = D3DXPlaneDotCoord(&m_planes[i], &center);
		if (distance < -radius)
		{
			m_cullingNum++;
			return false;
		}
	}
	m_drawNum++;
	return true;
}

//=============================================================
// 終了
//=============================================================
void OcclusionCulling::Uninit()
{
	// 破棄する
	for (auto drawData : m_drawDatas)
	{
		for (int i = 0; i < 2; i++)
		{
			drawData->query[i]->Release();
			drawData->query[i] = nullptr;
		}

		delete drawData;
		drawData = nullptr;
	}
	m_drawDatas.clear();
}

//=============================================================
// 結果を計算する
//=============================================================
void OcclusionCulling::Result()
{
	int counter = 0;
	int cullnum = 0;
	for (auto drawData : m_drawDatas)
	{
		int prevIdx = 1 - drawData->currentIdx;
		drawData->lastResult = true;

		// 結果を取得
		DWORD visiblePixels = 0;
		HRESULT result = drawData->query[prevIdx]->GetData(&visiblePixels, sizeof(DWORD), 0);
		
		// 成功
		if (result == S_OK)
		{
			drawData->lastResult = (visiblePixels > 0);
			if (!drawData->lastResult) cullnum++;
		}

		// 次のインデックスにする
		drawData->currentIdx = prevIdx;

		counter++;
	}
}

//=============================================================
// 描画するかを取得する
//=============================================================
bool OcclusionCulling::IsDraw(GameObject* obj)
{
	for (auto data : m_drawDatas)
	{
		if (data->obj == obj)
		{
			return data->lastResult;
		}
	}

	return false;
}

//=============================================================
// データをリセットする
//=============================================================
void OcclusionCulling::ResetData()
{
	for (auto itr = m_drawDatas.begin(); itr != m_drawDatas.end(); itr++)
	{
		//(*itr)->lastResult = false;
		(*itr)->life++;

		// 寿命が尽きたとき
		if ((*itr)->life >= LIFE)
		{
			OcclusionData* data = *itr;
			
			// 破棄する
			for (int i = 0; i < 2; i++)
			{
				data->query[i]->Release();
				data->query[i] = nullptr;
			}
			delete data;
			data = nullptr;

			// リストから除外する
			itr = m_drawDatas.erase(itr);
			if (itr == m_drawDatas.end())
			{
				return;
			}
		}
	}
}

//=============================================================
// データをセットする
//=============================================================
void OcclusionCulling::SetData(GameObject* obj)
{
	for (auto drawData : m_drawDatas)
	{
		// 既にデータが存在しているとき
		if (drawData->obj == obj)
		{
			drawData->life = 0;
			return;
		}
	}

	// 新規作成
	auto device = Manager::GetInstance()->GetDevice();

	OcclusionData* data = new OcclusionData();
	for (int i = 0; i < 2; i++)
	{
		if (FAILED(device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &data->query[i])))
		{
			if (i == 1)
			{
				data->query[0]->Release();
			}

			delete data;
			data = nullptr;
			return;
		}
	}

	data->obj = obj;
	data->lastResult = true;
	data->currentIdx = 0;
	data->life = 0;

	m_drawDatas.push_back(data);
}

//=============================================================
// クエリを取得する
//=============================================================
IDirect3DQuery9* OcclusionCulling::GetQuery(GameObject* obj)
{
	for (auto drawData : m_drawDatas)
	{
		if (drawData->obj == obj)
		{
			return drawData->query[drawData->currentIdx];
		}
	}

	return nullptr;
}
