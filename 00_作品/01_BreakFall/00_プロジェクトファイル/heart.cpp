//==========================================================================================================
//
// ハートの処理[heart.cpp]
// Author ItouKotaro
//
//==========================================================================================================
#include "heart.h"
#include "manager.h"
#include "square.h"
#include "transform.h"
#include "sound.h"

// 定数
constexpr const char* HEART_TEX = "heart_00.png";
constexpr const char* HEART_EMPTY_TEX = "heart_01.png";
const float HeartManager::HEART_SIZE = 70.0f;
const float HeartManager::HEART_SPACE = HEART_SIZE + 10.0f;

//=======================================================================================================
//初期化処理
//=======================================================================================================
void HeartUI::Init()
{
	//テクスチャを変更する
	this->GetComponent<Square>()->SetTexture(Manager::Get()->GetTexture()->Get(m_active ? HEART_TEX : HEART_EMPTY_TEX));
}

//=======================================================================================================
//更新処理
//=======================================================================================================
void HeartUI::SetActive(const bool& active)
{
	if (active != m_active)
	{ //変更されたとき
		//テクスチャを変更する
		this->GetComponent<Square>()->SetTexture(Manager::Get()->GetTexture()->Get(active ? HEART_TEX : HEART_EMPTY_TEX));
	
		//状態を更新する
		m_active = active;
	}
}

//=======================================================================================================
//初期化処理
//=======================================================================================================
void HeartManager::Init()
{
	//ハートUIを生成する
	for (int i = 0; i < MAX_HEART; i++)
	{
		//ハートテスト
		Object* pHeart = new Object("HeartUI");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pHeart);
		pTransform2D->SetPos(this->GetComponent<Transform2D>()->GetPos() + D3DXVECTOR2(HEART_SPACE * i, 0.0f));
		pTransform2D->SetSize({ HEART_SIZE, HEART_SIZE });
		pHeart->AddComponent(pTransform2D);

		//四角形レンダリングを追加する
		Square* pSquare = new Square(pHeart);
		pHeart->AddComponent(pSquare);

		//ハートUIを追加する
		HeartUI* pHeartUI = new HeartUI(pHeart);
		pHeart->AddComponent(pHeartUI);

		//初期化する
		pHeart->Init();

		//配列に代入する
		m_heartUI[i] = pHeart;
	}
}

//=======================================================================================================
//更新処理
//=======================================================================================================
void HeartManager::Update()
{
	//体力を反映する
	for (int i = 0; i < MAX_HEART; i++)
	{
		m_heartUI[i]->GetComponent<HeartUI>()->SetActive(i < m_value);
	}
}

//=======================================================================================================
//ハート回復
//=======================================================================================================
void HeartManager::RecoveryHeart()
{
	if (m_value < MAX_HEART)
	{//回復の余地があるとき
		m_value++;
	}
}

//=======================================================================================================
//ハートダメージ
//=======================================================================================================
void HeartManager::DamageHeart()
{
	if (m_value > 0)
	{//ダメージの余地があるとき
		m_value--;

		// ダメージ音を鳴らす
		PlaySound(SOUND_LABEL_DAMAGE);

		// 体力が無くなったとき
		if (m_value <= 0)
		{
			m_isAlive = false;
		}
	}
}

//=======================================================================================================
//ハートダメージ
//=======================================================================================================
void HeartManager::AllDamageHeart()
{
	while (m_value > 0)
	{
		DamageHeart();
	}
}
