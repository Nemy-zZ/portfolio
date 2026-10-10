//=============================================================
//
// プレゼントジェネレータ [present_generator.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PRESENT_GENERATOR_H_
#define _PRESENT_GENERATOR_H_

class PresentGenerator
{
public:
	void Init();
	void Uninit();
	void Update();
private:
	void Generate();
	float m_nextGenerator;
};

#endif // !_PRESENT_GENERATOR_H_
