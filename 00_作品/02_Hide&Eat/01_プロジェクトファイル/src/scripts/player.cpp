//=============================================================
//
// プレイヤー [player.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "player.h"
#include "component/3d/collision.h"
#include "component/2d/bar.h"
#include "component/2d/text.h"
#include "component/3d/motion.h"
#include "component/other/sound.h"

//=============================================================
// [PlayerControl] 初期化
//=============================================================
void PlayerControl::Init()
{
	m_camera = GameObject::Find("Camera");

	// レベルアップバー
	m_lvBar = new GameObject();
	m_lvBar->AddComponent<CBar>();
	m_lvBar->GetComponent<CBar>()->SetBold(50.0f);
	m_lvBar->GetComponent<CBar>()->GetPolyBar()->SetColor(D3DCOLOR_RGBA(0, 255, 255, 255));
	m_lvBar->GetComponent<CBar>()->GetPolyBG()->transform->SetSize(810.0f, 60.0f);
	m_lvBar->GetComponent<CBar>()->GetPolyBG()->transform->SetPos(-5.0f, -5.0f);
	m_lvBar->transform->SetPos(50.0f, 50.0f);

	// レベルアップバーの背景
	GameObject* lvBarBG = new GameObject();
	lvBarBG->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	lvBarBG->transform->SetSize(820.0f, 70.0f);
	lvBarBG->SetParent(m_lvBar);
	lvBarBG->transform->SetPos(-10.0f, -10.0f);
	lvBarBG->SetPriority(5);

	// レベルテキスト
	m_lvText = new GameObject();
	m_lvText->AddComponent<CText>()->SetText("Lv." + std::to_string(m_level));
	m_lvText->GetComponent<CText>()->SetFont("ラノベPOP v2");
	m_lvText->GetComponent<CText>()->SetFontSize(130);
	m_lvText->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(226, 174, 0, 255));
	m_lvText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_lvText->GetComponent<CText>()->SetOutlineSize(3);
	m_lvText->transform->SetPos(900.0f, 40.0f);
}

//=============================================================
// [PlayerControl] 更新
//=============================================================
void PlayerControl::Update()
{
	float axisX = 0.0f;
	float axisY = 0.0f;

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

	// 操作
	if (INPUT_INSTANCE->onPress("a")) axisX += -1.0f;
	if (INPUT_INSTANCE->onPress("d")) axisX += 1.0f;
	if (INPUT_INSTANCE->onPress("w")) axisY += 1.0f;
	if (INPUT_INSTANCE->onPress("s")) axisY += -1.0f;

	// ハイド
	m_hide = INPUT_INSTANCE->onPress("space") || INPUT_INSTANCE->onPress("p:a");
	if (m_hide)
	{
		gameObject->GetComponent<CMotionManager>()->Play("hiding");
		return;
	}

	// 方向を計算する
	float angle = 0.0f;
	if (controlStickLx == 0 && controlStickLy == 0)
	{
		angle = atan2f(axisX, axisY) + Benlib::PosAngle(m_camera->transform->GetWPos(), transform->GetWPos());
	}
	else
	{
		angle = atan2f(controlStickLx, controlStickLy) + Benlib::PosAngle(m_camera->transform->GetWPos(), transform->GetWPos());
	}
	

	// 移動
	float speed = 1.0f - m_level * 0.05f;
	if ((axisX != 0.0f || axisY != 0.0f) || (controlStickLx != 0 || controlStickLy != 0))
	{
		gameObject->GetComponent<CMotionManager>()->Play("move");
		transform->Translate(sinf(angle) * speed, 0.0f, cosf(angle) * speed);

		if (m_soundCounter == 29)
		{
			GameObject* sound = new GameObject;
			sound->AddComponent<CSound>()->LoadWav("data\\SE\\move_00.wav");
			sound->GetComponent<CSound>()->IsStoppedDestroy();
			sound->GetComponent<CSound>()->Play();
		}
	}
	else
	{
		gameObject->GetComponent<CMotionManager>()->Play("neutral");
	}

	// プレイヤーの回転を設定する
	transform->SetRot(0.0f, angle + D3DX_PI, 0.0f);

	// レベルのUI
	m_progress += ((m_score % 7) / (float)7 - m_progress) * 0.08f;
	m_lvBar->GetComponent<CBar>()->SetProgress(m_progress);


	m_soundCounter++;
	if (m_soundCounter >= 30)
	{
		m_soundCounter = 0;
	}
}

//=============================================================
// [PlayerControl] スコアの加算
//=============================================================
void PlayerControl::AddScore(const int& score)
{
	// スコアを加算する
	m_score += score;

	int oldLv = m_level;

	// レベルを計算する
	m_level = 1 + (m_score - (m_score % 7)) / 7;
	m_lvText->GetComponent<CText>()->SetText("Lv." + std::to_string(m_level));

	if (m_level != oldLv)
	{
		GameObject* sound = new GameObject();
		sound->AddComponent<CSound>()->LoadWav("data\\SE\\lvup_00.wav");
		sound->GetComponent<CSound>()->IsStoppedDestroy();
		sound->GetComponent<CSound>()->Play();
	}

	if (m_level == 8)
	{
		GameObject* sound = new GameObject();
		sound->AddComponent<CSound>()->LoadWav("data\\SE\\caneat_00.wav");
		sound->GetComponent<CSound>()->IsStoppedDestroy();
		sound->GetComponent<CSound>()->Play();
	}

	// プレイヤーの更新
	UpdatePlayer();
}

//=============================================================
// [PlayerControl] プレイヤーの更新
//=============================================================
void PlayerControl::UpdatePlayer()
{
	gameObject->GetComponent<CBoxCollider>()->SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f) * (m_level * 0.3f + 1));
	gameObject->GetComponent<CBoxCollider>()->SetOffset(D3DXVECTOR3(0.0f, 5.0f * (m_level * 0.3f + 1), 0.0f));
	CCollision::GetCollision(gameObject)->UpdateCollision();
	CCollision::GetCollision(gameObject)->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);

	gameObject->transform->SetScale(0.1f * (m_level * 0.3f + 1));
}