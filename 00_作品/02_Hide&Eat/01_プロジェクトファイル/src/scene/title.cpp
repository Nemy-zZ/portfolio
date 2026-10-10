//=============================================================
//
// タイトル [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "renderer.h"

#include "render/shadow_rb.h"

//=============================================================
// [CTitleScene] 初期化
//=============================================================
void CTitleScene::Init()
{
	m_tutorial = false;

	m_tutorialImg = new GameObject();
	m_tutorialImg->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\tutorial.png");
	m_tutorialImg->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_tutorialImg->SetPriority(10);
	m_tutorialImg->SetVisible(false);

	// 背景
	GameObject* pBG = new GameObject;
	pBG->AddComponent<CPolygon>();
	pBG->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\title.png");
	pBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));

	// 選択肢を生成する
	for (int i = 0; i < SELECT::MAX; i++)
	{
		// 項目の表示名を格納する
		std::string sShowName;
		switch (i)
		{
		case SELECT::START:
			sShowName = "開始";
			break;
		case SELECT::EXIT:
			sShowName = "終了";
			break;
		}

		// オブジェクトを生成する
		m_pSelectObj[i] = new GameObject;
		m_pSelectObj[i]->AddComponent<CText>();
		m_pSelectObj[i]->GetComponent<CText>()->SetFont("ラノベPOP v2");
		m_pSelectObj[i]->GetComponent<CText>()->SetText("<size=200><color=255,255,255>" + sShowName);
		m_pSelectObj[i]->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
		m_pSelectObj[i]->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH) / 2 + 480.0f, 400.0f + i * 250.0f);
	}

	// レンダーバッファを設定する
	CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
	static_cast<CameraRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetCamera(nullptr);
}

//=============================================================
// [CTitleScene] 終了
//=============================================================
void CTitleScene::Uninit()
{
}

//=============================================================
// [CTitleScene] 更新
//=============================================================
void CTitleScene::Update()
{
	// 選択の更新
	UpdateSelect();

	// 選択したときの処理
	if (INPUT_INSTANCE->onTrigger("space") ||
		INPUT_INSTANCE->onTrigger("enter") ||
		INPUT_INSTANCE->onTrigger("p:a"))
	{
		switch (m_select)
		{
		case CTitleScene::START:

			if (m_tutorial)
			{
				CSceneManager::GetInstance()->SetScene("game");
			}

			if (!m_tutorial)
			{
				m_tutorialImg->SetVisible(true);
				m_tutorial = true;
			}

			break;
		case CTitleScene::EXIT:
			DestroyWindow(CRenderer::GetInstance()->GetHWND());
			break;
		}
	}
}

//=============================================================
// [CTitleScene] 描画
//=============================================================
void CTitleScene::Draw()
{
}


//=============================================================
// [CTitleScene] 選択の更新
//=============================================================
void CTitleScene::UpdateSelect()
{
	// コントローラー用
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;

	// コントローラーのデッドゾーン
	if ((controlStickLx <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		controlStickLx > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(controlStickLy <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			controlStickLy > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		controlStickLx = 0;
		controlStickLy = 0;
	}

	// 上選択
	if (INPUT_INSTANCE->onTrigger("w") ||
		INPUT_INSTANCE->onTrigger("up") ||
		controlStickLy > 0.0f)
	{
		if (m_select > 0)
		{
			m_select = static_cast<SELECT>(m_select - 1);
		}
	}

	// 下選択
	if (INPUT_INSTANCE->onTrigger("s") ||
		INPUT_INSTANCE->onTrigger("down") ||
		controlStickLy < 0.0f)
	{
		if (m_select < SELECT::MAX - 1)
		{
			m_select = static_cast<SELECT>(m_select + 1);
		}
	}

	// UIの更新
	for (int i = 0; i < SELECT::MAX; i++)
	{
		m_pSelectObj[i]->GetComponent<CText>()->SetAlpha(i == m_select ? 1.0f : 0.2f);
	}
}
