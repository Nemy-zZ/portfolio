//=============================================================
//
// ƒvƒŒƒ[ƒ“ƒg [present.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "present.h"
#include "player.h"
#include "component/other/sound.h"

//=============================================================
// [Present] ƒqƒbƒg
//=============================================================
void Present::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		other->GetComponent<PlayerControl>()->AddScore(1);
		gameObject->Destroy();

		GameObject* sound = new GameObject;
		sound->AddComponent<CSound>()->LoadWav("data\\SE\\eat_00.wav");
		sound->GetComponent<CSound>()->IsStoppedDestroy();
		sound->GetComponent<CSound>()->Play();
	}
}