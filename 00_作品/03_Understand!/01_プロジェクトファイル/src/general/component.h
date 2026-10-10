//------------------------------------------------------------
// @file		component.h
// @brief	コンポーネント
//------------------------------------------------------------
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "gameobject.h"

//@brief コンポーネントクラス
class Component : public Object
{
public:
	Component() : gameObject(nullptr), transform(nullptr) {};
	virtual ~Component() = default;

	/**
	 * @brief 初期化処理
	 * @details コンポーネントが追加された際に呼ばれるイベント
	*/
	virtual void Init() {}

	/**
	 * @brief 終了処理
	 * @details コンポーネントが破棄された際に呼ばれるイベント [強制]
	*/
	virtual void Uninit() {}

	/**
	 * @brief 更新処理
	 * @details 毎フレーム呼ばれるイベント
	*/
	virtual void Update() {}

	/**
	 * @brief オブジェクト描画処理
	 * @details オブジェクト描画の際に呼ばれるイベント
	*/
	virtual void Draw() {}

	/**
	 * @brief UI描画処理
	 * @details UI描画の際に呼ばれるイベント
	*/
	virtual void DrawUI() {}

	/**
	 * @brief 他のコリジョンと接触しているときに呼ばれるイベント
	*/
	virtual void OnCollisionStay(Collision* other) {}

	/**
	 * @brief 他のコリジョンと接触したときのイベント
	*/
	virtual void OnCollisionEnter(Collision* other) {}

	/**
	 * @brief 他のコリジョンと離れたときのイベント
	*/
	virtual void OnCollisionExit(Collision* other) {}

	/**
	 * @brief トリガーと他のコリジョンが重なっているときに呼ばれるイベント
	*/
	virtual void OnTriggerStay(Collision* other) {}

	/**
	 * @brief トリガーに他のコリジョンが入ってきたときのイベント
	*/
	virtual void OnTriggerEnter(Collision* other) {}

	/**
	 * @brief トリガーから他のコリジョンが出たときのイベント
	*/
	virtual void OnTriggerExit(Collision* other) {}

	/**
	 * @brief 全コンポーネントを取得する
	 * @param[in] onlyActive : アクティブ状態のみ
	*/
	static std::vector<Component*> GetComponents(bool onlyActive = false) {
		std::vector<Component*> result;
		auto gameObjects = GameObject::GetAllGameObjects();
		for (auto objItr = gameObjects.begin(); objItr != gameObjects.end(); objItr++)
		{
			auto components = (*objItr)->GetComponents();
			for (auto itr = components.begin(); itr != components.end(); itr++)
			{
				if (!onlyActive || (onlyActive && (*itr)->GetActive() && (*itr)->gameObject->GetActive()))
					result.push_back(*itr);
			}
		}
		return result;
	}

	/**
	 * @brief 全コンポーネントから複数の特定コンポーネントを取得する
	 * @param[in] includeChild : 子クラスを含めるか
	 * @param[in] onlyActive : アクティブ状態のみ
	*/
	template<class T> static std::vector<T*> GetComponents(const bool& includeChild = false, const bool& onlyActive = false) {
		std::vector<T*> result;
		auto components = Component::GetComponents();
		for (auto itr = components.begin(); itr != components.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
				{
					if (!onlyActive || (onlyActive && (*itr)->GetActive() && (*itr)->gameObject->GetActive()))
						result.push_back((T*)*itr);
				}
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(**itr))
				{
					if (!onlyActive || (onlyActive && (*itr)->GetActive() && (*itr)->gameObject->GetActive()))
						result.push_back((T*)*itr);
				}
			}
		}
		return result;
	}

	/**
	 * @brief ゲームオブジェクトにアタッチする
	 * @deprecated 自動で実行するため、自己での実行は非推奨です
	 * @param[in] attachObj : アタッチ先のゲームオブジェクト
	*/
	void AttachGameObject(GameObject* attachObj);

	/**
	 * @brief オブジェクトをデタッチする
	 * @deprecated 自動で実行するため、自己での実行は非推奨です
	*/
	void DetachGameObject();

	GameObject* gameObject;		// アタッチ先のゲームオブジェクト
	Transform* transform;			// アタッチ先のトランスフォーム
private:
	//@brief 解放処理
	void Release() override;
};

#endif // !_COMPONENT_H_
