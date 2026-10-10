//------------------------------------------------------------
// @file		object.h
// @brief	オブジェクト
// @detail	オブジェクト類の基本クラス
//------------------------------------------------------------
#ifndef _OBJECT_H_
#define _OBJECT_H_

//@brief オブジェクトクラス
class Object
{
public:
	Object();
	virtual ~Object();

	//@brief アクティブ状態を設定する
	void SetActive(bool value) { m_active = value; }
	//@brief アクティブ状態を取得する
	virtual bool GetActive() { return m_active; }

	//@brief デスフラグのオブジェクトを破棄する
	static void ReleaseDeathFlag();

	//@brief すべてのオブジェクトを破棄する
	static void AllDestroy();

	//@brief オブジェクトを破棄する
	static void Destroy(Object* obj) { obj->m_deathFlag = true; }

protected:
	//@brief 解放処理
	virtual void Release() = 0;
private:
	bool m_active;										// アクティブ状態
	bool m_deathFlag;									// デスフラグ
	static std::vector<Object*> m_objects;	// オブジェクトリスト
};

#endif // !_OBJECT_H_

