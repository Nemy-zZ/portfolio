//=============================================================
//
// ゲームシーン [game.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "game.h"
#include "component/3d/camera.h"
#include "component/3d/light.h"
#include "component/3d/collision.h"
#include "component/3d/field.h"
#include "component/3d/mesh.h"
#include "component/2d/text.h"
#include "component/other/sound.h"
#include "renderer.h"

#include "scripts/player.h"
#include "scripts/camera_move.h"
#include "scripts/enemy.h"

#include "scene/result.h"
#include "scene/ranking.h"

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	m_gameTime = 0.0f;
	m_isGameOver = false;
	m_isClear = false;

	// カメラ
	m_pCamera = new GameObject("Camera", "Camera");
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->GetComponent<CCamera>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pCamera->transform->SetPos(0.0f, 0.0f, -50.0f);
	m_pCamera->transform->LookAt({ 0.0f, 0.0f, 0.0f });
	m_pCamera->GetComponent<CCamera>()->GetSkybox()->LoadSkybox("data\\SKYBOX\\darknight.json");

	// ライトを作成
	GameObject* pLight = new GameObject();
	CD3DLight::SetDefaultD3DLight(pLight);

	// フィールドの生成
	GameObject* pField = new GameObject();
	pField->AddComponent<CField>()->Set(500.0f, 500.0f);
	pField->GetComponent<CField>()->SetTexture("data\\TEXTURE\\field_00.png");
	pField->GetComponent<CField>()->SetLoopTexture(8);
	pField->AddComponent<CBoxCollider>(D3DXVECTOR3(250.0f, 1.0f, 250.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));

	// 壁の生成
	GameObject* pWall0 = new GameObject();
	pWall0->AddComponent<CField>()->Set(500.0f, 250.0f);
	pWall0->GetComponent<CField>()->SetTexture("data\\TEXTURE\\wall_00.png");
	pWall0->GetComponent<CField>()->SetLoopTexture(8);
	pWall0->AddComponent<CBoxCollider>(D3DXVECTOR3(250.0f, 1.0f, 125.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	pWall0->transform->SetRot(D3DX_PI / 2, 0.0f, 0.0f);
	pWall0->transform->SetPos(D3DXVECTOR3(0.0f, 125.0f, -250.0f));

	GameObject* pWall1 = new GameObject();
	pWall1->AddComponent<CField>()->Set(500.0f, 250.0f);
	pWall1->GetComponent<CField>()->SetTexture("data\\TEXTURE\\wall_00.png");
	pWall1->GetComponent<CField>()->SetLoopTexture(8);
	pWall1->AddComponent<CBoxCollider>(D3DXVECTOR3(250.0f, 1.0f, 125.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	pWall1->transform->SetRot(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	pWall1->transform->SetPos(D3DXVECTOR3(-250.0f, 125.0f, 0.0f));

	GameObject* pWall2 = new GameObject();
	pWall2->AddComponent<CField>()->Set(500.0f, 250.0f);
	pWall2->GetComponent<CField>()->SetTexture("data\\TEXTURE\\wall_00.png");
	pWall2->GetComponent<CField>()->SetLoopTexture(8);
	pWall2->AddComponent<CBoxCollider>(D3DXVECTOR3(250.0f, 1.0f, 125.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	pWall2->transform->SetRot(D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	pWall2->transform->SetPos(D3DXVECTOR3(250.0f, 125.0f, 0.0f));

	GameObject* pWall3 = new GameObject();
	pWall3->AddComponent<CField>()->Set(500.0f, 250.0f);
	pWall3->GetComponent<CField>()->SetTexture("data\\TEXTURE\\wall_00.png");
	pWall3->GetComponent<CField>()->SetLoopTexture(8);
	pWall3->AddComponent<CBoxCollider>(D3DXVECTOR3(250.0f, 1.0f, 125.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	pWall3->transform->SetRot(D3DX_PI / 2, D3DX_PI, 0.0f);
	pWall3->transform->SetPos(D3DXVECTOR3(0.0f, 125.0f, 250.0f));

	// プレイヤーを生成する
	m_pPlayer = GameObject::LoadPrefab("data\\PREFAB\\player.pref");
	m_pPlayer->transform->SetPos(0.0f, 50.0f, 100.0f);
	//m_pPlayer->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\player.x");
	m_pPlayer->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 5.0f, 5.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	m_pPlayer->AddComponent<CRigidBody>()->EnableAlwayActive();
	m_pPlayer->AddComponent<PlayerControl>()->UpdatePlayer();

	// プレゼントジェネレータを生成する
	m_presentGenerator = new PresentGenerator();
	m_presentGenerator->Init();

	GameObject* pEnemy0 = new GameObject();
	pEnemy0->transform->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));
	pEnemy0->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\santa_oyazi_00.x");
	pEnemy0->AddComponent<CCapsuleCollider>(12.0f, 24.0f, D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	pEnemy0->AddComponent<CRigidBody>()->EnableAlwayActive();
	pEnemy0->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	pEnemy0->AddComponent<Enemy>();
	//pEnemy->transform->SetScale(0.5f);

	GameObject* pEnemy1 = new GameObject();
	pEnemy1->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\santa_oyazi_00.x");
	pEnemy1->AddComponent<CCapsuleCollider>(12.0f, 24.0f, D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	pEnemy1->AddComponent<CRigidBody>()->EnableAlwayActive();
	pEnemy1->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	pEnemy1->AddComponent<Enemy>();
	//pEnemy->transform->SetScale(0.5f);

	GameObject* pEnemy2 = new GameObject();
	pEnemy2->transform->SetPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	pEnemy2->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\santa_oyazi_00.x");
	pEnemy2->AddComponent<CCapsuleCollider>(12.0f, 24.0f, D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	pEnemy2->AddComponent<CRigidBody>()->EnableAlwayActive();
	pEnemy2->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	pEnemy2->AddComponent<Enemy>();
	//pEnemy->transform->SetScale(0.5f);

	// カメラの設定
	m_pCamera->AddComponent<CCameraMove>()->SetTarget(m_pPlayer);

	// レンダーバッファに登録
	CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(m_pCamera->GetComponent<CCamera>());
}

//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
	if (m_presentGenerator != nullptr)
	{
		m_presentGenerator->Uninit();
		delete m_presentGenerator;
		m_presentGenerator = nullptr;
	}
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	// プレゼントジェネレータの更新
	m_presentGenerator->Update();

	//if (INPUT_INSTANCE->onTrigger("space"))
	//{
	//	GameObject* pBench = GameObject::LoadPrefab("data\\PREFAB\\bench.pref", Transform({ 0.0f, 10.0f, 0.0f }));
	//}
	if (!m_isGameOver && !m_isClear)
	{
		m_gameTime += CManager::GetInstance()->GetDeltaTime();
	}
	else
	{
		if (INPUT_INSTANCE->onTrigger("enter") || INPUT_INSTANCE->onTrigger("p:a"))
		{
			if (m_isGameOver)
				CSceneManager::GetInstance()->SetScene("ranking");
			else if (m_isClear)
				CSceneManager::GetInstance()->SetScene("result");
		}
	}


}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{
}

//=============================================================
// [CGameScene] ゲームオーバー時
//=============================================================
void CGameScene::onGameOver()
{
	m_isGameOver = true;

	GameObject* pGameOver = new GameObject();
	pGameOver->AddComponent<CText>()->SetText("ゲームオーバー");
	pGameOver->GetComponent<CText>()->SetAlign(CText::CENTER);
	pGameOver->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2);

	GameObject* sound = new GameObject;
	sound->AddComponent<CSound>()->LoadWav("data\\SE\\failure_00.wav");
	sound->GetComponent<CSound>()->IsStoppedDestroy();
	sound->GetComponent<CSound>()->Play();

	static_cast<CRankingScene*>(CSceneManager::GetInstance()->GetScene("ranking")->pScene)->SetRank(0);
}

//=============================================================
// [CGameScene] ゲームクリア時
//=============================================================
void CGameScene::onClear()
{
	m_isClear = true;

	GameObject* pClear = new GameObject();
	pClear->AddComponent<CText>()->SetText("クリア");
	pClear->GetComponent<CText>()->SetAlign(CText::CENTER);
	pClear->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2);

	static_cast<CResultScene*>(CSceneManager::GetInstance()->GetScene("result")->pScene)->SetScore(static_cast<int>(m_gameTime));
}