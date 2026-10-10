//=============================================================
//
// カメラの動き [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"

#include "manager.h"
#include "component/3d/camera.h"

//=============================================================
// [CCameraMove] 初期化
//=============================================================
void CCameraMove::Init()
{
	m_cameraRot = { 0.0f, 0.0f, 0.0f };
}

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::Update()
{
	// ターゲットが存在するか確認する
	if (m_pTarget == nullptr)
	{ // ターゲットが存在しないとき
		return;
	}

	// カーソル
	if (GetActiveWindow() == CManager::GetInstance()->GetHWND())
	{
		// 現在のカーソル位置を取得する
		CManager::CursorPos cursor = CManager::GetInstance()->GetCursorClientPos();

		// 前回のカーソル位置と異なるとき
		if (cursor.x != m_oldCursor.x || cursor.y != m_oldCursor.y)
		{ // 前回の位置と異なるとき
			m_cameraRot.y += (cursor.x - m_oldCursor.x) * 0.002f;
			m_cameraRot.x += (cursor.y - m_oldCursor.y) * 0.002f;
		}

		// ウィンドウ外にマウスが行ったとき
		CManager::GetInstance()->SetCursorClientPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), static_cast<float>(CRenderer::SCREEN_HEIGHT / 2));
		m_oldCursor = CManager::GetInstance()->GetCursorClientPos();
	}

	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickRX = pGamepadDev->GetState().Gamepad.sThumbRX;
	short stickRY = pGamepadDev->GetState().Gamepad.sThumbRY;
	if (GetActiveWindow() == CManager::GetInstance()->GetHWND())
	{
		if (stickRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += -0.05f;
		}
		if (stickRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.y += 0.05f;
		}
		if (stickRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += 0.02f;
		}
		if (stickRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_cameraRot.x += -0.02f;
		}
	}

	UpdateTPS();

	transform->SetPos(m_posS);

	//カメラの視点移動を制限したいです！
	if (this->m_cameraRot.x <= 0.1f)
	{
		this->m_cameraRot.x = 0.1f;
	}
	if (this->m_cameraRot.x >= D3DX_PI - 1.0f)
	{
		this->m_cameraRot.x = D3DX_PI - 1.0f;
	}
}

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::UpdateTPS()
{
	// カメラの取得
	CCamera* pCamera = gameObject->GetComponent<CCamera>();
	pCamera->SetCustomPosR(true);
	pCamera->SetPosR(m_pTarget->transform->GetWPos() + D3DXVECTOR3(0.0f, 9.0f, 0.0f));

	// 正規化
	if (m_cameraRot.x < -D3DX_PI * 0.5f)
	{
		m_cameraRot.x = -D3DX_PI * 0.5f;
	}
	if (m_cameraRot.x > D3DX_PI * 0.5f)
	{
		m_cameraRot.x = D3DX_PI * 0.5f;
	}

	// 視点の位置を計算する
	float distance = 100.0f;
	D3DXVECTOR3 posS = { 0.0f, 0.0f, -distance };
	D3DXMATRIX mtxY, mtxX, mtxS;
	D3DXMatrixRotationX(&mtxX, m_cameraRot.x);
	D3DXMatrixRotationY(&mtxY, m_cameraRot.y);
	mtxS = mtxX * mtxY;
	D3DXVec3TransformCoord(&posS, &posS, &mtxS);

	posS += m_pTarget->transform->GetWPos();
	m_posS = posS;
}


//=============================================================
// [CCameraMove] オブジェクトが破棄されたときの処理
//=============================================================
void CCameraMove::OnDestroyOtherObject(GameObject* other)
{
	if (other == m_pTarget)
	{ // ターゲットのとき
		m_pTarget = nullptr;
	}
}