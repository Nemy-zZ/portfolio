//========================================================================================================
//
//プレイヤー処理[player.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "behavior.h"

//プレイヤー
class Player : public Behavior
{
public:
	static const int MAX_INSECT;   //虫の最大数
	static const float MOVE_SPEED;   //移動速度

	//モーションの列挙
	enum MOTION
	{
		IS_NEUTRAL = 0,
		IS_MOVE,
		IS_ATTACK,
	};

	//コンストラクタ
	Player(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_motionPlay = Player::MOTION::IS_NEUTRAL;   //再生中のモーション
		this->m_pCursor = nullptr;   //カーソル
		this->m_vInsect.clear();   //虫のコンテナ
		this->m_pNumInsect = nullptr;   //虫の数
	}

	//デストラクタ
	~Player() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理
	void EndMotion() override;   //モーション終了時の処理

private:
	void Control();   //コントロール処理
	void SearchInsect();   //虫を探す
	void ThrowInsect();   //虫を投げる
	void ChangeMotion(Player::MOTION motion);

	//虫を持っているか確認
	bool HasInsect(Object* pInsect)
	{
		//虫の数分繰り返す
		for (unsigned int i = 0; i < this->m_vInsect.size(); i++)
		{
			//虫を確認する
			if (this->m_vInsect[i] == pInsect)
			{
				//持っているを返す
				return true;
			}
		}

		//持っていないを返す
		return false;
	}

	Player::MOTION m_motionPlay;   //再生中のモーション
	Object* m_pCursor;  //カーソル
	std::vector<Object*> m_vInsect;   //虫のコンテナ
	Object* m_pNumInsect;   //虫の数のUI
};

#endif // !_PLAYER_H_
