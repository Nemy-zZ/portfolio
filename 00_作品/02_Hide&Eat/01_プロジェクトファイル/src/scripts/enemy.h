//=============================================================
//
// ìG [enemy.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "component.h"
#include "component/3d/collision.h"

class Enemy : public Component
{
public:
	Enemy() : m_state(PATROL), m_patrolRotationTimer(0.0f)/* : m_detectionTimer(0.0f), m_moveTimer(0.0f)*/{}
	void Init() override;
	void Update() override;

	static const float FOLLOW_RANGE;
	static const float DETECTION_DISTANCE;
	static const float MOVE_SPEED;
	//static const float FOLLOW_TIME;
private:
	void UpdateRay();
	void UpdateMove();

	// èÛë‘
	enum STATE
	{
		PATROL,
		FOLLOW,
	};
	STATE m_state;

	GameObject* m_player;
	GameObject* m_line[3];
	CCollision* m_hitPlayer;

	// ÉpÉgÉçÅ[Éãä÷åW
	float m_patrolRotationTimer;
	float m_patrolAngle;
	//float m_detectionTimer;
	//float m_moveTimer;
};

#endif // !_ENEMY_H_
