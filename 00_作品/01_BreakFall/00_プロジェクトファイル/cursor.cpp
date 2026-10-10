//========================================================================================================
//
//カーソルの処理[cursor.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "cursor.h"
#include "manager.h"
#include "benlib.h"
#include "plane.h"
#include "transform.h"

//静的メンバ変数を初期化する
const float Cursor::INIT_RANGE = 25.0f;
const float Cursor::ADD_RANGE = 5.0f;
const float Cursor::MOVE_SPEED = 20.0f;
const float Cursor::MAX_DISTANCE = 200.0f;

//========================================================================================================
//初期化処理
//========================================================================================================
void Cursor::Init()
{

}

//========================================================================================================
//更新処理
//========================================================================================================
void Cursor::Update()
{
	//プレイヤーを確認する
	if (this->m_pPlayer == nullptr)
	{
		//プレイヤーを取得する
		this->m_pPlayer = Object::Find("Player");
	}

	//操作処理を行う
	this->Control();
}

//========================================================================================================
//操作処理
//========================================================================================================
void Cursor::Control()
{
	//ジョイパッドを取得する
	Joypad* pJoypad = Manager::Get()->GetJoypad();

	//トンランスフォームを取得する
	Transform* pTransform = this->GetObj()->GetComponent<Transform>();

	//左スティックの入力情報を取得する
	short sThumbLX = pJoypad->GetState(0)->Gamepad.sThumbLX;   //左右入力
	short sThumbLY = pJoypad->GetState(0)->Gamepad.sThumbLY;   //上下入力

	//倒してる方向を計算する
	float fDireThumb = atan2f(sThumbLX, sThumbLY);

	//左スティック入力を入力を確認する
	if (sThumbLX != 0 || sThumbLY != 0)
	{
		//移動する
		pTransform->AddPos({ sinf(fDireThumb) * Cursor::MOVE_SPEED, 0.0f, cosf(fDireThumb) * Cursor::MOVE_SPEED });

		//プレイヤーとの距離を確認する
		D3DXVECTOR3 posCursor = pTransform->GetPos();
		D3DXVECTOR3 posPlayer = this->m_pPlayer->GetComponent<Transform>()->GetPos();
		if (Benlib::Distance(posCursor, posPlayer).y > Cursor::MAX_DISTANCE)
		{//プレイヤーとの距離の最大値を超えている場合
			//プレイヤーの方向を計算する
			float fDirePlayer = Benlib::Direction(posCursor, posPlayer).y + D3DX_PI;

			//最大値に補正する
			pTransform->SetPos({ posPlayer.x + sinf(fDirePlayer) * Cursor::MAX_DISTANCE, 0.01f, posPlayer.z + cosf(fDirePlayer) * Cursor::MAX_DISTANCE });
		}
	}

	//Aボタンの入力を確認する
	if (pJoypad->GetPress(Joypad::JOYKEY::IS_A, 0))
	{//Aボタンが押された場合
		//サーチ範囲を広げる
		this->m_fRange += Cursor::ADD_RANGE;
		if (this->m_fRange > Cursor::MAX_DISTANCE)
		{//サーチ範囲が規定値を超えた場合
			//調整する
			this->m_fRange = Cursor::MAX_DISTANCE;
		}
	}
	else
	{//押されていない場合
		//初期サイズに戻す
		this->m_fRange = Cursor::INIT_RANGE;
	}

	//サイズを設定する
	float fScale = (this->m_fRange * 2.0f) / Plane::BASE_SIZE.x;
	pTransform->SetScale({ fScale, 1.0f, fScale });
}