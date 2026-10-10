//------------------------------------------------------------
// @file		object.cpp
// @brief	オブジェクト
// @detail	オブジェクト類の基底クラス
//------------------------------------------------------------
#include "object.h"
#include "component.h"

// 静的メンバ変数の初期化
std::vector<Object*> Object::m_objects;

//=============================================================
// コンストラクタ
//=============================================================
Object::Object() : m_deathFlag(false), m_active(true)
{
	// データを追加する
	m_objects.push_back(this);
}

//=============================================================
// デストラクタ
//=============================================================
Object::~Object()
{
}

//=============================================================
// デスフラグのオブジェクトを破棄する
//=============================================================
void Object::ReleaseDeathFlag()
{
	int deathCounter = 0;	// 破棄したデスフラグのカウント
	do
	{
		// デスカウンターをリセットする
		deathCounter = 0;

		auto itr = m_objects.begin();
		while (itr != m_objects.end())
		{
			Object* obj = *itr;
			if (obj->m_deathFlag)
			{
				obj->Release();
				delete obj;
				itr = m_objects.erase(itr);

				deathCounter++;
			}
			else
			{
				itr++;
			}
		}
	} while (deathCounter > 0);
}

//=============================================================
// すべてのオブジェクトを破棄する
//=============================================================
void Object::AllDestroy()
{
	for (auto itr = m_objects.begin(); itr != m_objects.end(); itr++)
	{
		Destroy(*itr);
	}
	
	// デスフラグのオブジェクトを破棄する
	ReleaseDeathFlag();
}
