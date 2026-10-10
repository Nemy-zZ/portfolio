//========================================================================================================
//
//コンポーネントの処理[component.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "object.h"

//コンポーネント
class Component
{
public:
	//コンストラクタ
	Component(Object* pObject)
	{
		//メンバ変数を初期化する
		this->m_pObject = pObject;   //オブジェクト

		//コンテナに保存する
		Component::m_vComponent.push_back(this);
	}

	//デストラクタ
	virtual ~Component()
	{
		//コンポーネントの数分繰り返す
		for (unsigned int i = 0; i < Component::m_vComponent.size(); i++)
		{
			//コンポーネントを確認する
			if (Component::m_vComponent[i] != this)
			{//違うコンポーネントの場合
				//次のループに移行する
				continue;
			}

			//コンテナから削除する
			Component::m_vComponent.erase(Component::m_vComponent.begin() + i);
			break;   //ループを抜け出す
		}
	}
	
	virtual void Init() {};   //初期化処理
	virtual void Uninit() {};   //終了処理
	virtual void Update() {};   //更新処理
	virtual void Draw() {};   //描画処理

	Object* GetObj() { return this->m_pObject; };   //オブジェクトの取得
	template<class T> T* GetComponent() { return this->m_pObject->GetComponent<T>(); };   //コンポーネントの取得

	static std::vector<Component*> Get() { return Component::m_vComponent; };   //コンテナの取得
	
	//指定コンポーネントの取得
	template<class T> static std::vector<T*> Get()
	{
		std::vector<T*> vComponent;   //返す用コンテナ

		//オブジェクトの数分繰り返す
		for (unsigned int i = 0; i < Component::m_vComponent.size(); i++)
		{
			//指定のコンポーネントか確認する
			if (T* pComponent = dynamic_cast<T*>(Component::m_vComponent[i]))
			{//指定のコンポーネントの場合
				//返す用コンテナに保存する
				vComponent.push_back((T*)Component::m_vComponent[i]);
			}
		}

		//コンテナを返す
		return vComponent;
	}

private:
	Object* m_pObject;

	static std::vector<Component*> m_vComponent;   //コンテナ
};

#endif // !_COMPONENT_H_
