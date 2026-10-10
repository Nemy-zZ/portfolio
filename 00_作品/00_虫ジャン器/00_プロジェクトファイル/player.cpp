//========================================================================================================
//
//プレイヤー処理[player.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "player.h"
#include "manager.h"
#include "Xfile.h"
#include "keyboard.h"
#include "joypad.h"
#include "camera.h"
#include "model.h"
#include "motioneer.h"
#include "transform.h"
#include "cursor.h"
#include "insect.h"
#include "num_insect.h"
#include "benlib.h"

//静的メンバ変数を初期化する
const int Player::MAX_INSECT = 100;   //虫の最大数
const float Player::MOVE_SPEED = 2.5f;   //移動速度

//========================================================================================================
//初期化処理
//========================================================================================================
void Player::Init()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void Player::Update()
{
	//カーソルを確認する
	if (this->m_pCursor == nullptr)
	{
		//カーソルを検索する
		this->m_pCursor = Object::Find("Cursor");
	}

	//虫の数を確認する
	if (this->m_pNumInsect == nullptr)
	{//ない場合
		//虫の数を検索する
		this->m_pNumInsect = Object::Find("NumInsect");
	}

	//コントロール処理を行う
	this->Control();

	//虫の数を設定する
	this->m_pNumInsect->GetComponent<NumInsect>()->SetNumInsect(this->m_vInsect.size());

	//モーションを適用する
	this->GetObj()->GetComponent<Motioneer>()->StartMotion(this->m_motionPlay);
}

//========================================================================================================
//コントロール処理
//========================================================================================================
void Player::Control()
{
	//ジョイパッドを取得する
	Joypad* pJoypad = Manager::Get()->GetJoypad();

	//左スティックの入力情報を取得する
	short sThumbLX = pJoypad->GetState(0)->Gamepad.sThumbLX;   //左右入力
	short sThumbLY = pJoypad->GetState(0)->Gamepad.sThumbLY;   //上下入力

	//左スティックの入力を確認する
	if (abs(sThumbLX) > 5000 || abs(sThumbLY) > 5000)
	{//左スティックが入力されている場合
		//倒してる方向を計算する
		float fDireThumbL = atan2f(sThumbLX, sThumbLY);

		//移動する
		Transform* pTransform = this->GetObj()->GetComponent<Transform>();
		pTransform->AddPos({ sinf(fDireThumbL) * Player::MOVE_SPEED, 0.0f, cosf(fDireThumbL) * Player::MOVE_SPEED });
		pTransform->SetRot({ 0.0f, fDireThumbL + D3DX_PI, 0.0f });    //移動方向に向く

		//移動モーションに変更する
		this->ChangeMotion(Player::MOTION::IS_MOVE);
	}
	else
	{//入力されていない場合
		//待機モーションに変更する
		this->ChangeMotion(Player::MOTION::IS_NEUTRAL);
	}

	//Aボタンの入力を確認する
	if (pJoypad->GetPress(Joypad::JOYKEY::IS_A, 0))
	{
		//虫を探す
		this->SearchInsect();
	}

	//Bボタンの入力を確認する
	if (pJoypad->GetTrigger(Joypad::JOYKEY::IS_B, 0))
	{
		//虫を投げる
		this->ThrowInsect();
	}

	//処理終了
	return;
}

//========================================================================================================
//虫を探す
//========================================================================================================
void Player::SearchInsect()
{
	//虫の挙動が付いているオブジェクトを取得する
	std::vector<Insect*> vInsect = Component::Get<Insect>();

	//虫の数分繰り返す
	for (unsigned int i = 0; i < vInsect.size(); i++)
	{
		//虫の距離を求める
		float fDisInsect = Benlib::Distance(this->m_pCursor->GetComponent<Transform>()->GetPos(), vInsect[i]->GetObj()->GetComponent<Transform>()->GetPos()).y;
		if (fDisInsect <= this->m_pCursor->GetComponent<Cursor>()->GetRange())
		{
			//虫を持っているか確認する
			if (this->HasInsect(vInsect[i]->GetObj()) == false)
			{
				//虫の数を確認する
				if (this->m_vInsect.size() < Player::MAX_INSECT)
				{
					//虫を呼ぶ
					vInsect[i]->Call(this->GetObj());
					this->m_vInsect.push_back(vInsect[i]->GetObj());   //コンテナに入れる
				}
			}
		}
	}
}

//========================================================================================================
//虫を投げる
//========================================================================================================
void Player::ThrowInsect()
{
	//虫を確認する
	if (this->m_vInsect.empty())
	{
		//処理を終了する
		return;
	}

	//先頭にいる虫を投げる
	Transform* pTransform = this->GetObj()->GetComponent<Transform>();   //トランスフォーム
	D3DXVECTOR3 pos = pTransform->GetPos();   //投げる位置
	float fDire = Benlib::Direction(pos, this->m_pCursor->GetComponent<Transform>()->GetPos()).y;   //投げる向き
	float fDist = Benlib::Distance(pos, this->m_pCursor->GetComponent<Transform>()->GetPos()).y;
	this->m_vInsect.front()->GetComponent<Insect>()->Throw({ pos.x, pos.y + 50.0f, pos.z }, fDire, fDist);
	pTransform->SetRot({ 0.0f, fDire + D3DX_PI, 0.0f });   //投げる方向に向く
	this->m_vInsect.erase(this->m_vInsect.begin());   //投げた虫をコンテナから削除
}

//========================================================================================================
//モーションの変更
//========================================================================================================
void Player::ChangeMotion(Player::MOTION motion)
{
	//再生中のモーションを確認する
	if (this->m_motionPlay == Player::MOTION::IS_ATTACK)
	{//攻撃のモーションの場合

	}
	else
	{//それ以外の場合
		//モーションを設定する
		this->m_motionPlay = motion;
	}
}

//========================================================================================================
//モーション終了時の処理
//========================================================================================================
void Player::EndMotion()
{
	//ニュートラルに戻す
	this->m_motionPlay = Player::MOTION::IS_NEUTRAL;
}