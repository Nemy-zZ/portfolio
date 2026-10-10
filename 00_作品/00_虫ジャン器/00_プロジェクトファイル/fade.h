//========================================================================================================
//
//フェード処理[fade.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "behavior.h"

//フェードクラス
class Fade : public Behavior   //(親)挙動
{
public:
	//コンストラクタ
	Fade(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Fade() override
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:
};
#endif // !_FADE_H_
