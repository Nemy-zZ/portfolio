//=======================================================================================================
//
//警告の処理[warning.h]
// Author MurataShinnosuke
//
//=======================================================================================================
#ifndef _WARNING_H_
#define _WARNING_H_

#include "behavior.h"

//警告の挙動
class Warning : public Behavior   //（親）挙動
{
public:
	static const float MOVE;   //移動量

	//コンストラクタ
	Warning(Object* pObject) : Behavior(pObject)
	{
	}

	//デストラクタ
	~Warning()
	{
	}

	void Init() override;   //初期化処理
	void Update() override;   //更新処理

private:

};

#endif // !_WARNING_H_
