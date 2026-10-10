//=============================================================
//
// ƒvƒŒƒ[ƒ“ƒg [present.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PRESENT_H_
#define _PRESENT_H_

#include "component.h"

class Present : public Component
{
public:
	void OnTriggerEnter(GameObject* other) override;
private:
};

#endif // !_PRESENT_H_
