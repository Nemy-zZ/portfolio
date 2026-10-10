//========================================================================================================
//
//TPS視点カメラの処理[camera_tps.cpp]
// Auhtor Murata Shinnosuke
//
//========================================================================================================
#include "camera_tps.h"
#include "camera.h"
#include "object.h"
#include "transform.h"
#include "manager.h"
#include "joypad.h"
#include "mouse.h"

//静的メンバ変数を初期化する
const float CameraTPS::DISTANCE_VIEW_TO_REGARD = 200.0f;   //視点の注視点の距離
const float CameraTPS::DISTANCE_TARGET_TO_FULCRUM = 0.0f;

//======================================================================================================
//初期化処理
//======================================================================================================
void CameraTPS::Init()
{
	//カメラを取得する
	this->m_pCamera = this->GetObj()->GetComponent<Camera>();
}

//======================================================================================================
//更新処理
//======================================================================================================
void CameraTPS::Update()
{
	//操作処理を行う
	this->Control();

	//エイムフラグを確認する
	if (this->m_isAim)
	{
		//視点を対象の位置にする
		this->GetObj()->GetComponent<Transform>()->SetPos(this->m_posTarget);

		//カメラの注視点を求める
		D3DXVECTOR3 posView = this->GetObj()->GetComponent<Transform>()->GetPos();
		D3DXVECTOR3 rotCamera = this->GetObj()->GetComponent<Transform>()->GetRot();
		D3DXVECTOR3 posRegard =
		{
			posView.x - CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * sinf(rotCamera.y),
			posView.y - CameraTPS::DISTANCE_VIEW_TO_REGARD * cosf(rotCamera.x),
			posView.z - CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * cosf(rotCamera.y),
		};
	}
	else
	{
		//カメラの向きを取得する
		D3DXVECTOR3 rotCamera = this->GetObj()->GetComponent<Transform>()->GetRot();

		//カメラの支点を求める
		this->m_posFulcrum =
		{
			this->m_posTarget.x - sinf(rotCamera.y + (D3DX_PI * 0.5f)) * CameraTPS::DISTANCE_TARGET_TO_FULCRUM,
			this->m_posTarget.y,
			this->m_posTarget.z - cosf(rotCamera.y + (D3DX_PI * 0.5f)) * CameraTPS::DISTANCE_TARGET_TO_FULCRUM
		};

		//カメラの視点を求める
		D3DXVECTOR3 posView =
		{
			this->m_posFulcrum.x + CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * sinf(rotCamera.y),
			this->m_posFulcrum.y + CameraTPS::DISTANCE_VIEW_TO_REGARD * cosf(rotCamera.x),
			this->m_posFulcrum.z + CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * cosf(rotCamera.y),
		};

		//カメラの注視点を求める
		D3DXVECTOR3 posRegard =
		{
			this->m_posFulcrum.x - CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * sinf(rotCamera.y),
			this->m_posFulcrum.y - CameraTPS::DISTANCE_VIEW_TO_REGARD * cosf(rotCamera.x),
			this->m_posFulcrum.z - CameraTPS::DISTANCE_VIEW_TO_REGARD * sinf(rotCamera.x) * cosf(rotCamera.y),
		};

		//情報を設定する
		this->GetObj()->GetComponent<Transform>()->SetPos(posView);
		this->m_pCamera->SetPosRegard(posRegard);
	}
}

//======================================================================================================
//操作処理
//======================================================================================================
void CameraTPS::Control()
{
	//--------------------------------------------------------------------------------------------------
	//ジョイパッドの操作処理
	//--------------------------------------------------------------------------------------------------
	{
		//ジョイパッドを取得する
		Joypad* pJoypad = Manager::Get()->GetJoypad();

		//右スティックの入力情報を取得する
		short sThumbRX = pJoypad->GetState(0)->Gamepad.sThumbRX;   //左右入力
		short sThumbRY = pJoypad->GetState(0)->Gamepad.sThumbRY;   //上下入力

		//左トリガーを確認する
		if (pJoypad->GetState(0)->Gamepad.bLeftTrigger != 0)
		{//左トリガーが押されている場合
			//エイムフラグを立てる
			this->m_isAim = true;
		}
		else
		{//押されていない場合
			//エイムフラグを下ろす
			this->m_isAim = false;
		}

		//倒してる方向を計算する
		float fDireThumbR = atan2f(sThumbRX, sThumbRY);
		if (sThumbRX != 0 || sThumbRY != 0)
		{//左スティックが入力されている場合
			//向きを加算する
			D3DXVECTOR3 rotAdd;
			rotAdd.x = cosf(fDireThumbR) * this->m_fSensi;
			rotAdd.y = sinf(fDireThumbR) * this->m_fSensi;
			this->GetObj()->GetComponent<Transform>()->AddRot(rotAdd);
		}
	}

	//--------------------------------------------------------------------------------------------------
	//マウスの操作処理
	//--------------------------------------------------------------------------------------------------
	{
		//マウスを取得する
		Mouse* pMouse = Manager::Get()->GetMouse();

		//マウスの移動量を確認する
		D3DXVECTOR2 moveMouse = pMouse->GetMove();
		if (moveMouse != D3DXVECTOR2(0.0f, 0.0f))
		{//入力されている場合
			//向きに加算する
			D3DXVECTOR3 rotAdd;
			rotAdd.x = (moveMouse.y * 0.001f);
			rotAdd.y = (moveMouse.x * -0.001f);
			this->GetObj()->GetComponent<Transform>()->AddRot(rotAdd);
		}
	}
}