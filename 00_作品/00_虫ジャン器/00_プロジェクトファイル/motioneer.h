//========================================================================================================
//
//モーショナーの処理[motioneer.h]
// Author MurataShinnosuke
//
//========================================================================================================
#ifndef _MOTIONEER_H_
#define _MOTIONEER_H_

#include "component.h"
#include "motion.h"

//前方宣言
class Model;   //モデル
//モーショナー
class Motioneer : public Component
{
public:
	//コンストラクタ
	Motioneer(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_vMotion.clear();    //モーションのコンテナ
		this->m_nMotionPlay = 0;   //再生中のモーションの番号
		this->m_pModel = nullptr;   //モデル
	}

	//デストラクタ
	~Motioneer()
	{

	}

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void LoadMotion(std::string sFileName);   //モーションの読み込み処理

	//モーションの再生処理
	void StartMotion(int nId)
	{
		//モーションの番号を確認する
		if (nId != this->m_nMotionPlay)
		{//現在モーションと違う場合
			//現在のモーションに適用する
			this->m_nMotionPlay = nId;

			//モーションをリセットする
			this->m_vMotion[this->m_nMotionPlay]->Reset();
		}
	}

private:
	std::vector<Motion*> m_vMotion;   //モーションのコンテナ
	int m_nMotionPlay;   //再生中のモーション
	Model* m_pModel;   //モデル
};

#endif // !_MOTIONEER_H_