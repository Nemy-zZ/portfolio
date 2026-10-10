//------------------------------------------------------------
// @file			camera_options.h
// @brief		カメラのオプション
//------------------------------------------------------------
#ifndef _CAMERA_OPTIONS_H_
#define _CAMERA_OPTIONS_H_

#include "component.h"

//@brief 描画オブジェクトの制限(カメラオプション)
class LimitObjects : public Component
{
public:
	//@brief 制限タイプ
	enum TYPE
	{
		WHITE,	// ホワイト (指定オブジェクトのみ描画)
		BLACK	// ブラック (指定オブジェクト以外描画)
	};
	LimitObjects() : m_type(WHITE), m_ignoreActive(false){}

	//@brief 制限タイプを設定する
	void SetLimitType(const TYPE& type) { m_type = type; }

	//@brief リストにオブジェクトを追加する
	void AddObject(GameObject* object);
	//@brief リストにオブジェクトを削除する
	void RemoveObject(GameObject* object);
	//@brief リストをリセットする
	void ResetObject();

	//@brief アクティブ状態を無視するか
	void SetIgnoreActive(const bool& enabled) { m_ignoreActive = enabled; }

	//@brief 描画する
	void LimitDraw();

	//@brief 描画するオブジェクトかを判定する（オクルージョンカリング用）
	bool IsDrawObject(GameObject* object);

private:
	TYPE m_type;
	bool m_ignoreActive;
	std::vector<GameObject*> m_objects;
};

/**
 * @brief フラスタムカリング
 * @details 視錐台に入っていないオブジェクトをカリングします
 */
class FrustumCulling : public Component
{
public:
	//@brief 視錐台情報を更新する
	void UpdateFrustum();

	//@brief カリング数を取得する
	int GetCullingNum() { return m_cullingNum; }

	//@brief 表示するか
	bool IsDraw(const D3DXVECTOR3& center, const float& radius);
private:
	D3DXPLANE m_planes[6];
	int m_cullingNum, m_drawNum;
};

/**
 * @brief オクルージョンカリング
 */
class OcclusionCulling : public Component
{
private:
	const int LIFE = 60;

public:
	void Uninit() override;

	//@brief 結果を計算する
	void Result();

	//@brief 描画するかを取得する
	bool IsDraw(GameObject* obj);

	//@brief 描画データをリセットする
	void ResetData();

	//@brief 描画データをセットする
	void SetData(GameObject* obj);

	//@brief クエリを取得する
	IDirect3DQuery9* GetQuery(GameObject* obj);
private:
	struct OcclusionData
	{
		GameObject* obj;

		IDirect3DQuery9* query[2];	// クエリ（切り替えて使う）
		int currentIdx;							// 現在のインデックス
		bool lastResult;						// 前回の結果

		int life;	// 使われなくなったら自動破棄
	};

	std::vector<OcclusionData*> m_drawDatas;
};

#endif // !_CAMERA_OPTIONS_H_
