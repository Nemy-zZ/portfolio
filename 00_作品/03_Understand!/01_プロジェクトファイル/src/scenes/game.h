//------------------------------------------------------------
// @file		game.h
// @brief	ゲームシーン
//------------------------------------------------------------
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//@brief ゲームシーン
class GameScene : public Scene
{
public:
	void Init() override;
	void Update() override;

	bool IsStarted() { return m_isStarted; }
	void SetStarted(const bool& value);
	GameObject* GetCamera() { return m_camera; }
	GameObject* GetBall() { return m_ball; }
	DWORD GetElapsedTime() { return m_startTime; }
	float GetAveSpeed();
private:
	GameObject* m_camera;
	GameObject* m_ball;
	bool m_isStarted;
	DWORD m_startTime;
	std::vector<float> m_speedStack;
	float m_recordTime;
	int m_stopCount;
	GameObject* m_stopText;
};

#endif // !_SCENE_GAME_H_
