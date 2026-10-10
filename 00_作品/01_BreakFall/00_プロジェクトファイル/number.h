//========================================================================================================
//
//数字の処理[number.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "rendering.h"
#include "digit.h"

//数字クラス
class Number : public Rendering   //(親)オブジェクト
{
public:
	//コンストラクタ
	Number(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_nNumber = 0;   //数字
		this->m_vDigid.clear();
	}

	//デストラクタ
	~Number() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;	  //終了処理
	void Update() override;	  //更新処理
	void Draw() override;	//描画処理

	void SetNumber(int nNumber);   //数字の設定処理
	void SetDigit(int nDigirt);   //桁数の設定処理

private:
	int m_nNumber;   //数字
	std::vector<Digit*> m_vDigid;   //桁
};

#endif // !_NUMBER_H_
