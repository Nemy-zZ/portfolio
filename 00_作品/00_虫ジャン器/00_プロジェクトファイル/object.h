//========================================================================================================
//
//オブジェクト処理[object.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//前方宣言
class Component;   //コンポーネント
//オブジェクトクラス
class Object
{
public:
	static const int PRIORITY = 6;   //優先度

	//タグの列挙
	enum TAG
	{
		IS_DEFAULT = 0,   //デフォルト
		IS_PLAYER,   //プレイヤー
		IS_FRUIT,   //フルーツ
		IS_JELLY,   //ゼリー
		IS_OBSTACLE,   //障害物
	};

	//コンストラクタ
	Object(std::string sName, int nPriority = 3)
	{
		//メンバ変数を初期化する
		this->m_sName = sName;   //名前
		this->m_tag = TAG::IS_DEFAULT;   //タグ
		this->m_nPriority = nPriority;   //優先度
		this->m_isDeath = false;   //死亡フラグ
		this->m_isActive = true;   //アクティブフラグ
		this->m_vComponent.clear();

		//オブジェクト管理の最後尾に保存する
		Object::m_vObject.push_back(this);
	}

	//デストラクタ
	virtual ~Object()
	{
		//コンテナから自身を削除する
		Object::m_vObject.erase(Object::m_vObject.begin() + this->GetOder());
	}

	virtual void Init();   //初期化処理
	virtual void Uninit();   //終了処理
	virtual void Update();   //更新処理
	virtual void Draw();   //描画処理
	void Death() { this->m_isDeath = true; };   //死亡フラグを立てる処理
	void AddComponent(Component* pComponent) { this->m_vComponent.push_back(pComponent); };   //コンポーネントの追加処理
	void SetTag(Object::TAG tag) { this->m_tag = tag; };   //タグの設定処理
	void SetActive(bool isActive) { this->m_isActive = isActive; };   //アクティブフラグの設定
	Object::TAG GetTag() { return this->m_tag; };   //タグの取得処理
	int GetPriority() { return this->m_nPriority; };   //優先度の取得処理
	bool GetActive() { return this->m_isActive; };   //アクティブフラグの取得

	//コンポーネントの取得処理
	template<class T> T* GetComponent()
	{
		//コンポーネントの数分繰り返す
		int nSize = this->m_vComponent.size();
		for (int i = 0; i < nSize; i++)
		{
			//指定のコンポーネントか確認する
			if (T* pComponent = dynamic_cast<T*>(this->m_vComponent[i]))
			{//指定のコンポーネントの場合
				//コンポーネントを返す
				return (T*)this->m_vComponent[i];
			}
		}

		//nullを返す
		return nullptr;
	}

	static void UpdateAll();   //一括更新
	static void DrawAll();   //一括描画
	static void DestroyAll();   //一括破棄
	static Object* Find(std::string sName);   //検索
	static Object* Get(int nId) { return m_vObject[nId]; };   //取得処理

private:
	//順番の取得処理
	int GetOder()
	{
		//オブジェクトの数分繰り返す
		int nSize = Object::m_vObject.size();
		for (int i = 0; i < nSize; i++)
		{
			//保存されたオブジェクトを確認する
			if (Object::m_vObject[i] != this)
			{//自身じゃない場合
				//次のループに移行する
				continue;
			}

			//周回を返す
			return i;
		}

		//-1を返す
		return -1;
	}

	std::string m_sName;   //名前
	TAG m_tag;   //タグ
	int m_nPriority;   //優先度
	bool m_isDeath;   //死亡フラグ
	bool m_isActive;   //アクティブフラグ
	std::vector<Component*> m_vComponent;

	static std::vector<Object*> m_vObject;   //コンテナ
};

#endif // !_OBJECT_H_