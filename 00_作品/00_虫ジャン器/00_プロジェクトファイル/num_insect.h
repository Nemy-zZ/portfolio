//========================================================================================================
//
//虫の数[insect_num.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _NUM_INSECT_H_
#define _NUM_INSECT_H_

#include "behavior.h"

//虫の数クラス
class NumInsect : public Behavior   //(親)挙動
{
public:
	//虫の数
	NumInsect(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_nNumInsect = 0;   //虫の数
	}

	//デストラクタ
	~NumInsect() override
	{
	}

	void Init() override;   //初期化
	void Update() override;   //更新処理
	void SetNumInsect(int nNum);   //虫の数の設定

private:
	int m_nNumInsect;   //虫の数
};

#endif // !_NUM_INSECT_H_