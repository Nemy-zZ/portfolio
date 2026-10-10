//------------------------------------------------------------
// @file		transform.h
// @brief	トランスフォーム
// @detail	位置や回転などの情報を持つクラス
//------------------------------------------------------------
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "d3dx9.h"

class GameObject;

//@brief トランスフォームクラス
class Transform final
{
public:
	Transform() 
	{
		position = {0.0f, 0.0f, 0.0f};
		D3DXQuaternionIdentity(&rotation);
		scale = { 1.0f, 1.0f, 1.0f };
		size = { 1.0f, 1.0f };
		m_parent = nullptr;
		m_attachObject = nullptr;
	}
	Transform(GameObject* attach);
	~Transform();

	//@brief 親を設定する
	void SetParent(Transform* parent);
	//@brief 親を取得する
	Transform* GetParent() { return m_parent; }

	//@brief アタッチされているゲームオブジェクトを取得する
	GameObject* GetAttachObject() { return m_attachObject; }

	//@brief 移動する
	void Translate(float x, float y, float z) { position += {x, y, z}; }
	//@brief 移動する
	void Translate(D3DXVECTOR3 add) { Translate(add.x, add.y, add.z); }

	//@brief 回転する
	void Rotate(float x, float y, float z);
	//@brief 回転する
	void Rotate(D3DXVECTOR3 add) { Rotate(add.x, add.y, add.z); }

	//@brief オイラー角で回転を取得する
	D3DXVECTOR3 GetEulerAngle();
	//@brief オイラー角で回転を設定する
	void SetEulerAngle(float x, float y, float z);
	//@brief オイラー角で回転を設定する
	void SetEulerAngle(D3DXVECTOR3 rotation) { SetEulerAngle(rotation.x, rotation.y, rotation.z); }
	//@brief オイラー角で回転を設定する
	void SetEulerAngle(float fAngle) { SetEulerAngle(GetEulerAngle().x, GetEulerAngle().y, fAngle); }

	//@brief 指定位置の方向に回転する
	void LookAt(float x, float y, float z);
	//@brief 指定位置の方向に回転する
	void LookAt(D3DXVECTOR3 at) { LookAt(at.x, at.y, at.z); }

	//@brief ワールド基準の位置を取得する
	D3DXVECTOR3 GetWorldPosition();
	//@brief ワールド基準の回転を取得する
	D3DXQUATERNION GetWorldRotation();
	//@brief ワールド基準のオイラー角を取得する
	D3DXVECTOR3 GetWorldEulerAngle();
	//@brief ワールド基準のスケールを取得する
	D3DXVECTOR3 GetWorldScale();

	//@brief マトリックスを取得する
	D3DXMATRIX& GetMatrix();
	//@brief 位置情報マトリックスを取得する
	D3DXMATRIX GetTranslationMatrix();
	//@brief 回転情報マトリックスを取得する
	D3DXMATRIX GetRotationMatrix();

	D3DXVECTOR3 position;			// 位置
	D3DXQUATERNION rotation;	// 回転
	D3DXVECTOR3 scale;				// スケール
	D3DXVECTOR2 size;				// サイズ

	//@brief 演算子 ==
	bool operator==(Transform value);

	//@brief 演算子 !=
	bool operator!=(Transform value);

	//@brief 演算子 =
	Transform& operator=(Transform value);

private:
	//@brief クォータニオンをオイラー角に変換する
	D3DXVECTOR3 QuaternionToEulerAngle(D3DXQUATERNION q);

	D3DXMATRIX m_matrix;				// マトリックス
	Transform* m_parent;				// 親
	GameObject* m_attachObject;	// アタッチオブジェクト
};

/**
 * @brief トランスフォーム監視
 * @details トランスフォームの値の変更を監視します
*/
class TransformMonitor final
{
public:
	//@brief コンストラクタ
	TransformMonitor();
	/**
	 * @brief コンストラクタ
	 * @param[in] target : 監視対象のトランスフォーム
	*/
	TransformMonitor(Transform* target);
	//@brief デストラクタ
	~TransformMonitor();

	/**
	 * @brief 監視対象を設定します
	 * @param[in] target : 監視対象のトランスフォーム
	*/
	void SetTarget(Transform* target);

	/**
	 * @brief 変更されたか
	 * @details 位置・回転・スケール・サイズのいずれかが変更されたとき
	*/
	bool HasChanged();
	//@brief 位置が変更されたか
	bool HasPositionChanged() { return m_changedPosition; }
	//@brief 回転が変更されたか
	bool HasRotationChanged() { return m_changedRotation; }
	//@brief スケールが変更されたか
	bool HasScaleChanged() { return m_changedScale; }
	//@brief サイズが変更されたか
	bool HasSizeChanged() { return m_changedSize; }

	//@brief デタッチする
	static void Detatch(Transform* target);
	//@brief 更新する
	static void AllUpdate();

private:
	//@brief 監視変数を更新する
	void UpdateMonitorValue();

	//@brief 更新する
	void Update();

	Transform* m_target;		// 監視対象

	// 監視フラグ
	bool m_changedPosition;
	bool m_changedRotation;
	bool m_changedScale;
	bool m_changedSize;
	
	// 監視変数
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR2 m_size;
	D3DXQUATERNION m_rotation;

	// 一覧
	static std::vector<TransformMonitor*> m_monitors;
};

#endif // !_TRANSFORM_H_
