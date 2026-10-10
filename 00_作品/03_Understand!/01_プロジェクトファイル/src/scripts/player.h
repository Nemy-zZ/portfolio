//------------------------------------------------------------
// @file		player.h
// @brief	プレイヤー本体です。
//------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "component.h"

class Player : public Component
{
public:
	static constexpr float MOVE_SPEED = 100.0f;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;

	void Controller();
	void CameraControl();

private:
	//@brief 接地処理
	void UpdateLanding();

	float m_yaw = 0;
	float m_pitch = 0;
};

#endif // !_COLLISION_TEST_H_
