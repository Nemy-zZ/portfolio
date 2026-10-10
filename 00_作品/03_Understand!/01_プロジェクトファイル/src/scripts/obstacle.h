//------------------------------------------------------------
// @file		obstacle.h
// @brief	è·äQï®
//------------------------------------------------------------
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

class Obstacle : public Component
{
public:
	void Init() override;
	void Update() override;

	enum OBSTACLETYPE
	{
		OBSTACLE_WHITE,
		OBSTACLE_BLACK
	};
	void SetType(const OBSTACLETYPE& type) { m_type = type; }

private:
	OBSTACLETYPE m_type;
	float m_destroyTimer;
};

#endif // !_OBSTACLE_H_
