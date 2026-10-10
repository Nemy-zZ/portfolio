//------------------------------------------------------------
// @file		obstacle_generator.h
// @brief	障害物ジェネレーター
//------------------------------------------------------------
#ifndef _OBSTACLE_GENERATOR_H_
#define _OBSTACLE_GENERATOR_H_

class ObstacleGenerator
{
public:
	static ObstacleGenerator& GetInstance()
	{
		static ObstacleGenerator instance;
		return instance;
	}

	void Init();
	void Update();

private:
	ObstacleGenerator() = default;

	void GenerateObject();

	float m_timer;
};

#endif // !_OBSTACLE_GENERATOR_H_
