//------------------------------------------------------------
// @file		title.h
// @brief	タイトルシーン
//------------------------------------------------------------
#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

//@brief タイトルシーン
class TitleScene : public Scene
{
public:
	void Init() override;
	void Update() override;
private:
	enum TUTORIAL
	{
		TUTORIAL_NONE,
		TUTORIAL_1,
		TUTORIAL_2,
		TUTORIAL_3,
		TUTORIAL_ENDED
	};
	TUTORIAL m_page;
	GameObject* m_tutorial;
	GameObject* m_pressKey;
};

#endif // !_SCENE_TITLE_H_
