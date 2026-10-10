//========================================================================================================
//
//ブロック処理[block.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "block.h"
#include "manager.h"
#include "rendering.h"
#include "transform.h"
#include "heart.h"
#include "benlib.h"
#include "prefab.h"
#include "sound.h"

//静的メンバ変数を初期化する
const float Block::MOVE = 0.1f;
const D3DXVECTOR3 Block::SIZE = { 100.0f, 50.0f, 0.0f };   //サイズ

//========================================================================================================
//初期化処理
//========================================================================================================
void Block::Init()
{
	//乱数で色を決定する
	int nRand = Benlib::CreateRand(4, 0);
	Rendering* pRendering = this->GetComponent<Rendering>();   //レンダリングの取得
	if (nRand == Block::COLOR::IS_RED)
	{//赤の場合
		//赤色にする
		pRendering->SetColor({ 1.0f, 0.25f, 0.25f, 1.0f });
	}
	else if (nRand == Block::COLOR::IS_BLUE)
	{//青の場合
		//青にする
		pRendering->SetColor({ 0.25f, 0.25f, 1.0f, 1.0f });
	}
	else if (nRand == Block::COLOR::IS_GREEN)
	{//緑の場合
		//緑にする
		pRendering->SetColor({ 0.25f, 1.0f, 0.25f, 1.0f });
	}
	else if (nRand == Block::COLOR::IS_YELLOW)
	{//黄の場合
		//黄にする
		pRendering->SetColor({ 1.0f, 1.0f, 0.25f, 1.0f });
	}

	//乱数で体力を設定する
	if (Benlib::CreateRand(2, 0) == 0)
	{
		//ヒットポイントを1に設定する
		this->m_nHp = 1;
		pRendering->SetTexture(Manager::Get()->GetTexture()->Get("block_02.png"));   //テクスチャ
	}
	else if (Benlib::CreateRand(2, 0) == 0)
	{
		//ヒットポイントを1に設定する
		this->m_nHp = 2;
		pRendering->SetTexture(Manager::Get()->GetTexture()->Get("block_01.png"));   //テクスチャ
	}
	else if (Benlib::CreateRand(2, 0) == 0)
	{
		//ヒットポイントを2に設定する
		this->m_nHp = 3;
		pRendering->SetTexture(Manager::Get()->GetTexture()->Get("block_00.png"));   //テクスチャ
	}
}

//========================================================================================================
//更新処理
//========================================================================================================
void Block::Update()
{
	//ブロックを移動させる
	this->GetComponent<Transform>()->AddPos({ 0.0f, -Block::MOVE, 0.0f });

	//ブロックの位置を確認する
	if (this->GetComponent<Transform>()->GetPos().y <= -SCREEN_HEIGHT / 2)
	{//画面外に行った場合
		//ハートを取得する
		Object* pHeart = Object::Find("HeartUI");

		//体力を0にする
		pHeart->GetComponent<HeartManager>()->AllDamageHeart();
	}
}

//=======================================================================================================
//ヒット処理
//=======================================================================================================
void Block::Hit()
{
	//ヒットポイントを減らす
	this->m_nHp--;
	Rendering* pRendering = this->GetComponent<Rendering>();   //レンダリングの取得
	if (this->m_nHp <= 0)
	{//ヒットポイントが０になったら
		//乱数でアイテムをドロップする
		if (Benlib::CreateRand(10, 0) == 0)
		{
			//乱数でアイテムを決める
			int nRand = Benlib::CreateRand(9, 0);
			if (nRand >= 8)
			{
				//？アイテムをドロップする
				ClearBlockItemPrefab::Create(this->GetComponent<Transform>()->GetPos());
			}
			else if (nRand >= 6)
			{
				//補助アイテムをドロップする
				AssistBarItemPrefab::Create(this->GetComponent<Transform>()->GetPos());
			}
			else if (nRand >= 3)
			{
				//ハート追加アイテムをドロップする
				AddHeartItemPrefab::Create(this->GetComponent<Transform>()->GetPos());
			}
			else if (nRand >= 0)
			{
				//ボール追加アイテムをドロップする
				AddBallItemPrefab::Create(this->GetComponent<Transform>()->GetPos());
			}
		}

		//自信を破棄する
		this->GetObj()->Death();
	}
	else if (this->m_nHp == 1)
	{
		//壊れそうのテクスチャを設定
		pRendering->SetTexture(Manager::Get()->GetTexture()->Get("block_02.png"));   //テクスチャ
	}
	else if (this->m_nHp == 2)
	{
		//ひび入りのテクスチャを設定
		pRendering->SetTexture(Manager::Get()->GetTexture()->Get("block_01.png"));   //テクスチャ
	}

	//音を鳴らす
	PlaySound(SOUND_LABEL_BLOCK_BREAK);
}