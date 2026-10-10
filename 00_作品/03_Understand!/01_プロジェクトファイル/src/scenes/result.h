//------------------------------------------------------------
// @file		result.h
// @brief	リザルトシーン
//------------------------------------------------------------
#ifndef _SCENE_RESULT_H_
#define _SCENE_RESULT_H_

//@brief リザルトシーン
class ResultScene : public Scene
{
public:
	void Init() override;
	void Update() override;
private:
	float m_elapsedTime;
};

#endif // !_SCENE_RESULT_H_
