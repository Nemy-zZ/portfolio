//=============================================================
//
// ゲームシーン [ranking.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ranking.h"
#include "scripts/ranking_system.h"
#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "renderer.h"

//=============================================================
// [CRankingScene] 初期化
//=============================================================
void CRankingScene::Init()
{
	RankingSystem rank;

	GameObject* bgObj = new GameObject();
	bgObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\result2.png");
	bgObj->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);


	// スコアを表示する
	for (int i = 0; i < RankingSystem::MAX_DATA; i++)
	{
		GameObject* scoreText = new GameObject();
		scoreText->AddComponent<CText>();
		scoreText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		scoreText->GetComponent<CText>()->SetOutlineSize(3);

		// 分と秒
		int min = (rank.GetScore(i) - rank.GetScore(i) % 60) / 60;
		int sec = rank.GetScore(i) % 60;

		// 時間の表示形式を変更する
		char* timeText = new char[128];
		if (i+1 == m_rank) sprintf(timeText, "<size=130>%d.  <size=150><color=255,145,0>%d:%02d", i+1, min, sec);
		else sprintf(timeText, "<size=130>%d.  <size=150>%d:%02d", i + 1, min, sec);

		scoreText->GetComponent<CText>()->SetText(timeText);
		
		scoreText->GetComponent<CText>()->SetFont("ラノベPOP v2");
		scoreText->transform->SetPos(780.0f, 220.0f + 150.0f * i);
	}

	// レンダーバッファを設定する
	CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CRankingScene] 終了
//=============================================================
void CRankingScene::Uninit()
{

}

//=============================================================
// [CRankingScene] 更新
//=============================================================
void CRankingScene::Update()
{
	if (INPUT_INSTANCE->onTrigger("enter") || INPUT_INSTANCE->onTrigger("p:a"))
	{
		CSceneManager::GetInstance()->SetScene("title");
	}
}

//=============================================================
// [CRankingScene] 描画
//=============================================================
void CRankingScene::Draw()
{

}