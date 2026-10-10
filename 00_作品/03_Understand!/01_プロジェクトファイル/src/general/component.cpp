//------------------------------------------------------------
// @file		component.cpp
// @brief	コンポーネント
//------------------------------------------------------------
#include "component.h"

//=============================================================
// ゲームオブジェクトにアタッチする
//=============================================================
void Component::AttachGameObject(GameObject* attachObj)
{
	if (gameObject == nullptr)
	{ // アタッチされていないとき
		gameObject = attachObj;
		transform = gameObject->transform;
	}
}

//=============================================================
// ゲームオブジェクトをデタッチする
//=============================================================
void Component::DetachGameObject()
{
	if (gameObject != nullptr)
	{ // アタッチされているとき
		GameObject* gameObjectData = gameObject;
		gameObject = nullptr;
		transform = nullptr;
		gameObjectData->DetachComponent(this);
	}
}

//=============================================================
// 解放する
//=============================================================
void Component::Release()
{
	// 終了処理を行う
	this->Uninit();

	// コンポーネントを解除する
	if (gameObject != nullptr)
	{
		gameObject->DetachComponent(this);
	}
}
