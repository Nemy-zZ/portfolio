//=============================================================
//
// リザルトシーン [result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "scripts/ranking_system.h"
#include "ranking.h"
#include "renderer.h"

//=============================================================
// [CResultScene] 初期化
//=============================================================
void CResultScene::Init()
{
	// リザルト背景
	GameObject* bgObj = new GameObject("BG");
	bgObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\result.png");
	bgObj->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// リザルトテキスト
	GameObject* resultText = new GameObject();
	resultText->AddComponent<CText>()->SetText("<size=200>リザルト");
	resultText->GetComponent<CText>()->SetFont("ラノベPOP v2");
	resultText->GetComponent<CText>()->SetAlign(CText::CENTER);
	resultText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 100.0f);

	// タイム
	// 分と秒
	int min = (m_score - m_score % 60) / 60;
	int sec = m_score % 60;

	// 時間の表示形式を変更する
	char* timeText = new char[128];
	sprintf(timeText, "<size=400>%d:%02d", min, sec);

	m_timeNumText = new GameObject();
	m_timeNumText->AddComponent<CText>()->SetText(timeText);
	m_timeNumText->GetComponent<CText>()->SetFont("ラノベPOP v2");
	m_timeNumText->GetComponent<CText>()->SetFontSize(200);
	m_timeNumText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_timeNumText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 450.0f);

	// スコアを登録する
	RankingSystem rankSystem;
	rankSystem.RegisterData(m_score);

	// ランキングにスコアを受け渡す
	static_cast<CRankingScene*>(CSceneManager::GetInstance()->GetScene("ranking")->pScene)->SetRank(rankSystem.GetRanking(m_score));

	// レンダーバッファを設定する
	CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CResultScene] 終了
//=============================================================
void CResultScene::Uninit()
{
}

//=============================================================
// [CResultScene] 更新
//=============================================================
void CResultScene::Update()
{
	if (INPUT_INSTANCE->onTrigger("enter") || INPUT_INSTANCE->onTrigger("p:a"))
	{
		CSceneManager::GetInstance()->SetScene("ranking");
	}
}

//=============================================================
// [CResultScene] 描画
//=============================================================
void CResultScene::Draw()
{
}
