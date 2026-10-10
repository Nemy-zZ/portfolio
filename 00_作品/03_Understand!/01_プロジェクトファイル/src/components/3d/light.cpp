//------------------------------------------------------------
// @file		light.cpp
// @brief	ライト
// @detail	3Dオブジェクトを表示するために必要なライト
//------------------------------------------------------------
#include "light.h"
#include "system/manager.h"

std::vector<D3DLight*> D3DLight::m_lights;

//=============================================================
// 初期化
//=============================================================
void D3DLight::Init()
{
	// ライトリストに追加する
	m_lights.push_back(this);
}

//=============================================================
// 終了
//=============================================================
void D3DLight::Uninit()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	// 最後尾のライトを無効にする
	device->LightEnable(static_cast<DWORD>(m_lights.size() - 1), FALSE);

	// ライトリストから削除する
	for (auto itr = m_lights.begin(); itr != m_lights.end(); itr++)
	{
		if (*itr == this)
		{
			m_lights.erase(itr);
			break;
		}
	}
}

//=============================================================
// 更新
//=============================================================
void D3DLight::Update()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = Manager::GetInstance()->GetDevice();

	for (int i = 0; i < (int)m_lights.size(); i++)
	{
		// ライトを設定する
		device->SetLight(i, &m_lights[i]->m_light);

		// ライトを有効にする
		device->LightEnable(i, TRUE);
	}
}

//=============================================================
// デフォルトのライト設定
//=============================================================
void D3DLight::SetDefaultLight(GameObject* gameObject)
{
	D3DLight* lightComp = gameObject->AddComponent<D3DLight>();
	lightComp->SetType(D3DLIGHT_DIRECTIONAL);
	lightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp->SetDirection(D3DXVECTOR3(0.22f, -0.87f, 0.44f));

	lightComp = gameObject->AddComponent<D3DLight>();
	lightComp->SetType(D3DLIGHT_DIRECTIONAL);
	lightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp->SetDirection(D3DXVECTOR3(-0.18f, 0.88f, -0.44f));

	lightComp = gameObject->AddComponent<D3DLight>();
	lightComp->SetType(D3DLIGHT_DIRECTIONAL);
	lightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp->SetDirection(D3DXVECTOR3(0.89f, 0.11f, 0.44f));
}
