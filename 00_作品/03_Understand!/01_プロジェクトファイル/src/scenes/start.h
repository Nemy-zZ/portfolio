//------------------------------------------------------------
// @file		start.h
// @brief	最初のシーン
// @detail	起動直後に呼ばれるシーンの処理です
//------------------------------------------------------------
#ifndef _SCENE_START_H_
#define _SCCENE_START_H_

#include "scene.h"

//@brief 開始シーン
class StartScene : public Scene
{
public:
	void Init() override;
};

#endif // !_SCENE_START_H_
