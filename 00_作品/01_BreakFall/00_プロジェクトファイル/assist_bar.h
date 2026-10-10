//=======================================================================================================
//
//アシスト棒の処理[assist_bar.h]
// Author MurataShinnosuke
//
//=======================================================================================================
#ifndef _ASSIST_BAR_H_
#define _ASSIST_BAR_H_

#include "behavior.h"

//アシスト棒の挙動
class AssistBar : public Behavior   //（親）挙動
{
public:
	//コンストラクタ
	AssistBar(Object* pObject) : Behavior(pObject)
	{
		this->m_nCntLife = 900;
	}

	//デストラクタ
	~AssistBar()
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:
	int m_nCntLife;
};

#endif // !_ASSIST_BAR_H_
