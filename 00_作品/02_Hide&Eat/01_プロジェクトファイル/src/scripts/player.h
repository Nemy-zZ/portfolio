//=============================================================
//
// ÉvÉåÉCÉÑÅ[ [player.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "component.h"

class PlayerControl : public Component
{
public:
	PlayerControl() : m_score(0), m_level(1), m_progress(0.0f), m_soundCounter(0), m_hide(false){}
	void Init() override;
	void Update() override;
	void AddScore(const int& score);
	bool GetIsHide() { return m_hide; }
	void UpdatePlayer();
	int GetLevel() { return m_level; }
private:
	GameObject* m_camera;
	GameObject* m_lvBar;
	GameObject* m_lvText;
	int m_score;
	int m_level;
	bool m_hide;
	float m_progress;
	int m_soundCounter;
};


#endif // !_PLAYER_H_
