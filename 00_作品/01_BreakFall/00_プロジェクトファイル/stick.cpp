//=======================================================================================================
//
//棒の処理[stick.cpp]
// Author MurataShinnosuke
//
//=======================================================================================================
#include "stick.h"
#include "manager.h"
#include "transform.h"
#include "rigidbody.h"
#include "collider.h"
#include "ball.h"
#include "sound.h"

//静的メンバ変数を初期化する
const float Stick::MOVE = 10.0f;   //移動量

//=======================================================================================================
//初期化処理
//=======================================================================================================
void Stick::Init()
{

}

//=======================================================================================================
//更新処理
//=======================================================================================================
void Stick::Update()
{
	//キーボードを取得する
	Keyboard* pKeyborad = Manager::Get()->GetKeyboard();

	//Aキーを確認する
	if (pKeyborad->GetPress(DIK_A))
	{
		//左に移動する
		Transform* pTransform = this->GetComponent<Transform>();
		pTransform->AddPos({ -Stick::MOVE, 0.0f, 0.0f });
		if (pTransform->GetPos().x <= -SCREEN_WIDTH / 2)
		{
			//位置を修正する
			pTransform->SetPos({ -SCREEN_WIDTH / 2 , pTransform->GetPos().y, 0.0f });
		}
	}

	//Dキーを確認する
	if (pKeyborad->GetPress(DIK_D))
	{
		//左に移動する
		Transform* pTransform = this->GetComponent<Transform>();
		pTransform->AddPos({ Stick::MOVE, 0.0f, 0.0f });
		if (pTransform->GetPos().x >= SCREEN_WIDTH / 2)
		{
			//位置を修正する
			pTransform->SetPos({ SCREEN_WIDTH / 2 , pTransform->GetPos().y, 0.0f });
		}
	}
}