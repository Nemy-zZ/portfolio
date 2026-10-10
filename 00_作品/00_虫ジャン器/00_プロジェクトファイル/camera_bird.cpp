//========================================================================================================
//
//バード視点カメラの処理[camera_bird.cpp]
// Auhtor Murata Shinnosuke
//
//========================================================================================================
#include "camera_bird.h"
#include "camera.h"
#include "transform.h"
#include "manager.h"
#include "joypad.h"
#include "mouse.h"

//静的メンバ変数を初期化する
const D3DXVECTOR3 CameraBird::OFFSET_POS_REGARD = { 0.0f, 500.0f, -250.0f };   //視点の注視点の距離

//======================================================================================================
//初期化処理
//======================================================================================================
void CameraBird::Init()
{
	//カメラを取得する
	this->m_pCamera = this->GetObj()->GetComponent<Camera>();
}

//======================================================================================================
//更新処理
//======================================================================================================
void CameraBird::Update()
{
	//プレイヤーを確認する
	if (this->m_pPlayer == nullptr)
	{
		//プレイヤーを取得する
		this->m_pPlayer = Object::Find("Player");
	}
	else
	{
		//カメラの位置をプレイヤーに合わせる
		D3DXVECTOR3 posPlayer = this->m_pPlayer->GetComponent<Transform>()->GetPos();
		this->m_pCamera->SetPosRegard(posPlayer);
		this->m_pCamera->GetObj()->GetComponent<Transform>()->SetPos({ posPlayer + CameraBird::OFFSET_POS_REGARD });
	}
}