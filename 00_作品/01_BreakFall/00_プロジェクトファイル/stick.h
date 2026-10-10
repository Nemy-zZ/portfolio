//=======================================================================================================
//
//棒の処理[stick.h]
// Author MurataShinnosuke
//
//=======================================================================================================
#ifndef _STICK_H_
#define _STICK_H_

#include "behavior.h"

//棒の挙動
class Stick : public Behavior   //（親）挙動
{
public:
	static const float MOVE;   //移動量

	//コンストラクタ
	Stick(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Stick()
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:

};

#endif // !_STICK_H_
