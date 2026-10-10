//------------------------------------------------------------
// @file		gameobject.h
// @brief	ゲームオブジェクト
// @detail	コンポーネントの情報を持ったオブジェクトです
//------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "system/object.h"
#include "transform.h"

// マクロ定義
#define TAG_NODESTROY		"NO_DESTROY"

// コンポーネントの宣言
class Component;

//@brief ゲームオブジェクト
class GameObject : public Object
{
public:
	static const int DEFAULT_PRIORITY = 6;
public:
	GameObject(std::string vName, std::string vTag, int priority = DEFAULT_PRIORITY);
	GameObject(std::string vName, int priority = DEFAULT_PRIORITY) : GameObject(vName, "", priority) {}
	GameObject(int priority = DEFAULT_PRIORITY) : GameObject("", "", priority) {}
	virtual ~GameObject();

	/**
	 * @brief 初期化
	 * @details ゲームオブジェクトに含まれるコンポーネントを初期化します（デフォルトでは使用されません）
	*/
	void Init();
	/**
	 * @brief 終了
	 * @details ゲームオブジェクトに含まれるコンポーネントを終了します（デフォルトでは使用されません）
	*/
	void Uninit();
	//@brief 更新
	void Update();
	//@brief 描画
	void Draw();
	//@brief UI描画
	void DrawUI();

	//@brief すべてのオブジェクトを更新する
	static void AllUpdate();
	//@brief すべてのオブジェクトを描画する
	static void AllDraw();
	//@brief すべてのオブジェクトをUI描画する
	static void AllDrawUI();

	//@brief コンポーネントを追加する
	template<class T, typename ...Args > inline T* AddComponent(Args... args) {
		// コンポーネントを作成する
		T* component = new T(args...);

		// ゲームオブジェクトにアタッチする
		component->AttachGameObject(this);

		// コンポーネントを追加する
		m_components.push_back(component);

		// コンポーネントの初期化処理を実行する
		component->Init();

		// 作成したコンポーネントを返す
		return component;
	}

	/*
	@brief コンポーネントを取得する
	@param[in] includeChild : 子クラスを含めるか
	*/
	template<class T> inline T* GetComponent(const bool& includeChild = false) {
		for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
					return (T*)*itr;
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(**itr))
					return (T*)*itr;
			}
		}
		return nullptr;
	}

	/*
	@brief 複数のコンポーネントを取得する
	@param[in] includeChild : 子クラスを含めるか
	*/
	template<class T> inline std::vector<T*> GetComponents(const bool& includeChild = false) {
		std::vector<T*> result;
		for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
					result.push_back((T*)*itr);
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(**itr))
					result.push_back((T*)*itr);
			}
		}
		return result;
	}

	//@brief すべてのコンポーネントを取得する
	std::vector<Component*>& GetComponents() {
		return m_components;
	}

	/**
	 * @brief 親を設定する
	 * @param[in] gameObject : ゲームオブジェクト
	*/
	void SetParent(GameObject* gameObject);

	/**
	 * @brief 親を取得する
	*/
	GameObject* GetParent();

	/**
	 * @brief 優先順位を設定する
	 * @param[in] priority : 優先順位 (処理順 小 -> 大)
	*/
	void SetPriority(const int& priority);

	//@brief アクティブ状態を取得する
	bool GetActive() override;

	//@brief コンポーネントをデタッチする
	void DetachComponent(Component* component);

	/**
	 * @brief 子オブジェクトを名前検索で取得する
	*/
	GameObject* FindChildrenByName(const std::string& name);

	/**
	 * @brief 子オブジェクトを取得する
	 * @details このオブジェクトが親に設定されているオブジェクトを取得します
	*/
	std::vector<GameObject*> GetChildren();

	/**
	 * @brief 一括りのオブジェクトリストを取得する
	 * @param[in] includeParent : 親をリストに含めるか
	*/
	std::vector<GameObject*> GetUnityObjects(const bool& includeParent = true);

	//@brief すべてのゲームオブジェクトを取得する
	static std::list<GameObject*>& GetAllGameObjects() { return m_gameObjects; }

	/**
	 * @brief 名前からゲームオブジェクトを取得する
	 * @return 見つかったゲームオブジェクト（未発見はnullptr）
	*/
	static GameObject* FindByName(const std::string& name);

	/**
	 * @brief タグからゲームオブジェクトを取得する
	 * @return 見つかったゲームオブジェクト（未発見はnullptr）
	*/
	static GameObject* FindByTag(const std::string& tag);

	/**
	 * @brief 名前とタグからゲームオブジェクトを取得する
	 * @return 見つかったゲームオブジェクト（未発見はnullptr）
	*/
	static GameObject* FindByNameTag(const std::string& name, const std::string& tag);

	/**
	 * @brief OBUSファイルからオブジェクトを生成する
	 * @param[in] path : パス（.obus）
	 * @param[in] transform : トランスフォーム
	 * @return プレハブのゲームオブジェクト
	*/
	static GameObject* CreateObuseObject(const std::string& path, const Transform& transform = Transform());

	Transform* transform;	// トランスフォーム情報
	std::string name;			// 名前
	std::string tag;				// タグ
private:
	//@brief 解放処理
	void Release() override;

	int m_priority;														// 優先順位
	std::vector<Component*> m_components;				// コンポーネント
	static std::list<GameObject*> m_gameObjects;		// すべてのゲームオブジェクト
};

#endif // !_GAMEOBJECT_H_
