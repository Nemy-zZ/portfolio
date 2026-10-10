//=============================================================
//
// プレゼントジェネレータ [present_generator.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "present_generator.h"
#include "manager.h"

#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/present.h"

//=============================================================
// [PresentGenerator] 初期化
//=============================================================
void PresentGenerator::Init()
{
	m_nextGenerator = 5.0f;

	for (int i = 0; i < 50; i++)
	{
		// プレゼントを生成する
		Generate();
	}
}

//=============================================================
// [PresentGenerator] 終了
//=============================================================
void PresentGenerator::Uninit()
{
}

//=============================================================
// [PresentGenerator] 更新
//=============================================================
void PresentGenerator::Update()
{
	// 次の生成
	//m_nextGenerator -= CManager::GetInstance()->GetDeltaTime();
	//if (m_nextGenerator <= 0.0f)
	//{
	//	// プレゼントを生成する
	//	Generate();
	//	m_nextGenerator = 5.0f;
	//}
}

//=============================================================
// [PresentGenerator] 生成
//=============================================================
void PresentGenerator::Generate()
{
	GameObject* pPresent = new GameObject();
	pPresent->transform->SetScale(0.1f);
	pPresent->transform->SetPos(Benlib::RandomFloat(-250.0f, 250.0f), 0.0f, Benlib::RandomFloat(-250.0f, 250.0f));
	pPresent->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\present_box_00.x");
	pPresent->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	CCollision::GetCollision(pPresent)->IsTrigger(true);
	pPresent->AddComponent<Present>();

}