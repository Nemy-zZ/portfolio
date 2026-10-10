//------------------------------------------------------------
// @file		ball.h
// @brief	ボール（プレイヤー）
//------------------------------------------------------------
#ifndef _BALL_H_
#define _BALL_H_

class Ball : public Component
{
public:
	static constexpr float BASE_SPEED = 20.0f;
	static constexpr float POWER = 0.0005f;
	static constexpr float POWERUP_TIME = 0.5f;

public:
	void Init() override;
	void Update() override;
	void OnCollisionEnter(Collision* other) override;

	enum BALLTYPE
	{
		BALL_WHITE,
		BALL_BLACK
	};
	BALLTYPE GetType() { return m_type; }
	void SetType(const BALLTYPE& type) { m_type = type; }

private:

	void StartingUpdate();

	BALLTYPE m_type;
	float m_power;
	float m_timer;
	float m_angle;

	GameObject* m_arrow;
	float m_arrowAngle;
};

#endif // !_BALL_H_
