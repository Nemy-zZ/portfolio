//=======================================================================================================
//
//ボールの処理[ball.cpp]
// Author MurataShinnosuke
//
//=======================================================================================================
#include "ball.h"
#include "transform.h"
#include "rigidbody.h"
#include "collider.h"
#include "block.h"
#include "heart.h"
#include "benlib.h"
#include "sound.h"

//静的メンバ変数を初期化する
const float Ball::MOVE = 6.0f;

//=======================================================================================================
//初期化処理
//=======================================================================================================
void Ball::Init()
{
	//移動量を加算する
	this->GetComponent<Rigidbody>()->SetMove({ sinf(0.0f) * Ball::MOVE, cosf(0.0f) * Ball::MOVE, 0.0f });
}

//=======================================================================================================
//更新処理
//=======================================================================================================
void Ball::Update()
{
	//ハートを確認する
	if (this->m_pHeart == nullptr)
	{
		//ハートを取得する
		this->m_pHeart = Object::Find("HeartUI");
	}

	//反射フラグを下ろす
	this->m_bReflection = false;

	//コンポーネントを取得する
	Transform* pTransform = this->GetObj()->GetComponent<Transform>();   //トランスフォーム
	Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();   //リジットボディ
	BoxCollider* pBoxCollider = this->GetObj()->GetComponent<BoxCollider>();   //ボックスコライダー

	//当たり判定の範囲を取得する
	BoxCollider::Range range = pBoxCollider->GetRange();

	//右の確認をする
	if (range.posMax.x > SCREEN_WIDTH / 2)
	{
		//位置を修正する
		D3DXVECTOR3 posNew = pTransform->GetPos();
		posNew.x = SCREEN_WIDTH / 2 - (pBoxCollider->GetSize().x / 2);
		this->GetObj()->GetComponent<Transform>()->SetPos(posNew);

		//反射する
		pRigidbody->AddMove({ -pRigidbody->GetMove().x * 2, 0.0f, 0.0f });
	}

	//左の確認をする
	if (range.posMin.x < -SCREEN_WIDTH / 2)
	{
		//位置を修正する
		D3DXVECTOR3 posNew = pTransform->GetPos();
		posNew.x = -SCREEN_WIDTH / 2 + (pBoxCollider->GetSize().x / 2);
		pTransform->SetPos(posNew);

		//反射する
		pRigidbody->AddMove({ -pRigidbody->GetMove().x * 2, 0.0f, 0.0f });
	}

	//上の確認をする
	if (range.posMax.y > SCREEN_HEIGHT / 2)
	{
		//位置を修正する
		D3DXVECTOR3 posNew = pTransform->GetPos();
		posNew.y = SCREEN_HEIGHT / 2 - (pBoxCollider->GetSize().y / 2);
		pTransform->SetPos(posNew);

		//反射する
		pRigidbody->AddMove({ 0.0f, -pRigidbody->GetMove().y * 2, 0.0f });
	}

	//下の確認をする
	if (range.posMin.y < -SCREEN_HEIGHT / 2)
	{
		//初期化する
		this->Init();
		this->GetComponent<Transform>()->SetPos({ 0.0f, 0.0f, 0.0f });   //位置

		//ダメージを与える
		this->Damage();
	}
}

//=======================================================================================================
//衝突時の処理
//=======================================================================================================
void Ball::Collision(Object* pObject, Physics::CONTACT contact)
{
	//衝突したオブジェクトのタグを確認する
	Object::TAG tagObject = pObject->GetTag();   //タグの取得
	if (tagObject == Object::TAG::IS_BLOCK || tagObject == Object::TAG::IS_ITEM)
	{//ボールの場合
		//リジットボディを取得する
		Rigidbody* pRigidbody = this->GetComponent<Rigidbody>();
		D3DXVECTOR3 move = pRigidbody->GetMove();   //移動量を取得

		//反射フラグを確認する
		if (this->m_bReflection == false)
		{//反射していない場合
			//衝突した位置を確認する
			switch (contact)
			{
			case Physics::CONTACT::IS_LEFT:   //左の場合
			case Physics::CONTACT::IS_RIGHT:   //右の場合

				//反射する
				move.x *= -1;   //X軸の移動量を反転する
				pRigidbody->SetMove(move);   //移動量を設定

				//処理を抜け出す
				break;

			case Physics::CONTACT::IS_TOP:   //上の場合
			case Physics::CONTACT::IS_BOTTOM:   //下の場合

				//反射する
				move.y *= -1;   //Y軸の移動量を反転する
				pRigidbody->SetMove(move);   //移動量を設定

				//処理を抜け出す
				break;
			}

			//反射フラグを立てる
			this->m_bReflection = true;
		}

		//タグを確認する
		if (tagObject == Object::TAG::IS_BLOCK)
		{
			//ブロックのヒット処理を行う
			pObject->GetComponent<Block>()->Hit();
		}
	}
	else if (tagObject == Object::TAG::IS_STICK)
	{//棒の場合
		//棒の方向を計算する
		float fDireStick = Benlib::Direction(this->GetComponent<Transform>()->GetPos(), pObject->GetComponent<Transform>()->GetPos()).z;

		//ボールをボールの方向に反射させる
		Rigidbody* pRigidbody = this->GetComponent<Rigidbody>();  //リジットボディを取得
		pRigidbody->SetMove({ -sinf(fDireStick) * Ball::MOVE, -cosf(fDireStick) * Ball::MOVE, 0.0f });

		//音を鳴らす
		PlaySound(SOUND_LABEL_STICK_HIT);
	}
	else if (tagObject == Object::TAG::IS_STICK_ASSIST)
	{
		//ボールを反射させる
		Rigidbody* pRigidbody = this->GetComponent<Rigidbody>();   //リジットボディ
		D3DXVECTOR3 moveBall = pRigidbody->GetMove();   //ボールの移動量
		moveBall.y *= -1;   //X軸反転
		pRigidbody->SetMove(moveBall);

		//音を鳴らす
		PlaySound(SOUND_LABEL_STICK_HIT);
	}
}

//=======================================================================================================
//ダメージ処理
//=======================================================================================================
void Ball::Damage()
{
	//ハートを減らす
	this->m_pHeart->GetComponent<HeartManager>()->DamageHeart();
}