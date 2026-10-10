//------------------------------------------------------------
// @file		scene.h
// @brief	シーン
// @detail	シーンの管理を行います
//------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_

//@brief シーン（基底）
class Scene
{
public:
	//@brief 初期化
	virtual void Init() {}
	//@brief 終了
	virtual void Uninit() {}
	//@brief 更新
	virtual void Update() {}
private:

};

//@brief シーン管理クラス
class SceneManager final
{
public:
	//@brief 初期化
	void Init();
	//@brief 更新
	void Update();

	//@brief シーンを変更する
	template<class T> bool SetScene(bool releaseObjects = true)
	{
		// 次のシーンをキューに入れる
		m_nextScene = GetScene<T>();
		m_isReleaseObjects = releaseObjects;

		// シーンの変更先が現在のシーンと一致するとき
		if (m_nextScene == m_currentScene)
		{
			Log::SendLog("シーンの変更先が現在のシーンと一致しています（" + static_cast<std::string>(typeid(T).name()) + "）", Log::TYPE_WARNING);
			return false;
		}

		// 成功
		return true;
	}

	//@brief シーンを取得する
	template<class T> T* GetScene()
	{
		// クラスが一致するシーンを見つける
		for (auto itr = m_scenes.begin(); itr != m_scenes.end(); itr++)
		{
			if (typeid(T) == typeid(**itr))
			{
				return static_cast<T*>(*itr);
			}
		}

		// 見つからなかったとき
		return nullptr;
	}

	//@brief シーンを登録する
	template<class T> bool RegisterScene()
	{
		// 既に登録されていないかを確認する
		for (auto itr = m_scenes.begin(); itr != m_scenes.end(); itr++)
		{
			if (typeid(T) == typeid(**itr))
			{ // 登録済みのとき
				Log::SendLog("登録済みのシーン \"" + static_cast<std::string>(typeid(T).name()) + "\" を再登録しようとしました", Log::TYPE_WARNING);
				return false;
			}
		}

		// 新しく登録する
		Scene* addScene = new T();
		m_scenes.push_back(addScene);
		Log::SendLog("シーン \"" + static_cast<std::string>(typeid(T).name()) + "\" を登録しました");

		return true;
	}

	//@brief シーンをすべて解放する
	void AllRelease();
private:
	//@brief ゲームオブジェクトを破棄する
	void ReleaseGameObjects();

	//@brief キューに入っているシーンに変更する
	void ChangeScene();

	Scene* m_currentScene;				// 現在のシーン
	Scene* m_nextScene;					// 次のシーン（キュー）
	bool m_isReleaseObjects;					// 次のシーンに移行するとき解放するか
	std::vector<Scene*> m_scenes;		// シーン
};

#endif // !_SCENE_H_
